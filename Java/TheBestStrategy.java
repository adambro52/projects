package planetwars.strategies;

import planetwars.publicapi.*;

import java.math.BigDecimal;
import java.util.*;
import java.util.List;
import org.jgrapht.*;
import org.jgrapht.graph.*;

public class TheBestStrategy implements IStrategy {

    /**
     * almostFull method needs to be refined and needs to be used within the nested for loop instead of right away
     *
     * need to make sure that home planets arent jerking each other off via a faulty check that they could use reinforcement
     *
     * it seems as if the computation cap is being imposed when the shortest path algorithm is called
     *
     * put in something to make sure I don't abandon a planet when i don't want to
     *
//
//    private HashMap<IVisiblePlanet, Integer> habitabilities;
//    private HashMap<IVisiblePlanet, Integer> sizes; */

    public void takeTurn(List<IPlanet> planets, IPlanetOperations planetOperations, Queue<IEvent> eventsToExecute) {

        double start = System.nanoTime(); // start time for efficient method

        ArrayList<IVisiblePlanet> ownedWorlds = getOwnedWorlds( planets );
        ArrayList<IVisiblePlanet> adjacentPlanets;

        for ( IVisiblePlanet ow : ownedWorlds ) { // goes through and I can now decide actions for each home world
            adjacentPlanets = planetSpecificAdjacentPlanets( ow, planets );

            int pop = (int) ow.getPopulation();

            int popNext = pop;

            if ( almostFull(ow) ){ // un-maximizationally handles a full planet by sending reinforcements

                IVisiblePlanet best = placeToSend( adjacentPlanets );

                if ( !almostFull(best) && best.getIncomingShuttles().size() == 0 ) {
                    eventsToExecute.add(planetOperations.transferPeople(ow, best, pop / 5));
                }
                else { /* this means that all planets surrounding this one are stocked
                                        so it should then send people in the direction of the enemy */
//                    IVisiblePlanet toTheEnemy = toTheFront( ow, planets );
//                    System.out.println("Dijkstra called");
//                    eventsToExecute.add(planetOperations.transferPeople(ow, toTheEnemy, pop / 10));

                    // this was gonna call dijkstras algorithm to find the closest path to the enemy but it didn't work
                }
            } else { // enters this if the planet is not almost full

                for (IVisiblePlanet p : adjacentPlanets) {

                    Owner owner = p.getOwner();

                    if (owner == Owner.SELF) {
                        if ( !amICapturable(ow, planets) && !almostFull( p ) ) {
                            if ( amICapturable(p, planets) || gonnaGetMurked( p ) ) {
                                eventsToExecute.add(planetOperations.transferPeople(ow, p, pop / 3));
                            }
                        }
                    } else if (owner == Owner.OPPONENT) {
                        int toCapture = (int) popToCapture( ow, p ) * 2;

                        if ( pop - toCapture >= 1 ) {

                                eventsToExecute.add(planetOperations.transferPeople( ow, p, toCapture ));
                                popNext = popNext - toCapture;

                        } else {
                           //eventsToExecute.add(planetOperations.transferPeople(ow, p, 1));
                            /* need to change this to attack more intelligently
                             * a good strategy would probs be to prioritize the attacks on the enemy planet with
                             * the most connections and/or ones that have a faster population growth rate  */
                        }
                    } else {
                        if ( p.getIncomingShuttles().size() == 0 && popNext > 1 ) { // makes sure not doubly sending ppl
                            // want it to send ppl until there is one person left on ow
                            eventsToExecute.add(planetOperations.transferPeople(ow, p, 1));
                            popNext--;
                        }
                    }
                }
            }
        }
        double end = System.nanoTime(); // end time for efficient method
        double Ctime = (end - start)/1000000; // convert nano to milli
        System.out.println("takeTurn method took " + Ctime + " milliseconds");
    }

    public ArrayList<IVisiblePlanet> planetSpecificAdjacentPlanets( IVisiblePlanet vp, List<IPlanet> planets ) {
        // gets adjecent planets to specific world

        ArrayList<IVisiblePlanet> adjacentPlanets = new ArrayList<>();

        Iterator<IEdge> it = vp.getEdges().iterator();
        while ( it.hasNext() ) {

            int id = it.next().getDestinationPlanetId();

            for ( IPlanet p : planets ) {
                if ( p.getId() == id ){
                    adjacentPlanets.add( (IVisiblePlanet) p );
                }
            }
        }
        return adjacentPlanets;
    }

    public ArrayList<IVisiblePlanet> getOwnedWorlds( List<IPlanet> planets ) {
        // gets owned worlds

        ArrayList<IVisiblePlanet> ownedWorlds = new ArrayList<>();

        for ( IPlanet p : planets ) {
            if ( p instanceof IVisiblePlanet ) {
                IVisiblePlanet converted = (IVisiblePlanet) p;
                if ( converted.getOwner() == Owner.SELF ) {
                    ownedWorlds.add(converted);
                }
            }
        }
        return ownedWorlds;
    }

    public HashMap<Integer, Integer> getDistances( IVisiblePlanet vp ){
        // enables easy interaction with distances between planets for a specific planet

        HashMap<Integer, Integer> distances = new HashMap<>();

        Iterator<IEdge> it = vp.getEdges().iterator();
        while ( it.hasNext() ) {
            IEdge holder = it.next();
            distances.put( holder.getDestinationPlanetId(), holder.getLength() );
        }
        return distances;
    }

    public int getDistance( IVisiblePlanet vp, IVisiblePlanet vp2 ){
        Iterator<IEdge> it = vp.getEdges().iterator();
        while (it.hasNext()){
            IEdge edge = it.next();
            if ( edge.getDestinationPlanetId() == vp2.getId() ){
                return edge.getLength();
            }
        }
        return 0;
    }

    public boolean amICapturable( IVisiblePlanet vulnerable, List<IPlanet> planets ){
        // true if total enemy population adjacent is big enough to take this one over

        boolean b = false;
        if ( vulnerable.getOwner() == Owner.OPPONENT ){ return b; }

        ArrayList<IVisiblePlanet> adjacents = planetSpecificAdjacentPlanets( vulnerable , planets );
        ArrayList<IVisiblePlanet> adjacentEnemies = new ArrayList<>();

        for ( IVisiblePlanet p : adjacents ) {
            if ( p.getOwner() == Owner.OPPONENT ){
                adjacentEnemies.add(p);
            }
        }

        int enemypop = 0;
        for ( IVisiblePlanet p : adjacentEnemies ) {
            enemypop += p.getPopulation();
        }

        if ( 1.1*enemypop > vulnerable.getPopulation() ){
            b = true;
        }

        return b;
    }

    public boolean gonnaGetMurked( IVisiblePlanet vp ){
        boolean b = false;
        int incoming = 0;
        List<IShuttle> shuttles = vp.getIncomingShuttles();

        for ( IShuttle s : shuttles ) {
            incoming += s.getNumberPeople();
        }

        if ( 1.1 * vp.getPopulation() <= incoming ){
            b = true;
        }
        return b;
    }

    public double popToCapture( IVisiblePlanet vp, IVisiblePlanet enemy ){
        // returns how many people that are needed to capture based on the distance and growth

        long pop = enemy.getPopulation();
        int dist = getDistance( vp, enemy ) * 2;
        int hab = enemy.getHabitability();

        while ( dist != 0 ){
            pop = pop * ( 1 + hab/100 );
            dist--;
        }

        return 1.1 * pop;
    }

    public long modelGrowth( int turns, long pop, int hab){
        // returns total population after a set number of turns

        if ( turns == 0 ){
            return (int) pop;
        } else {
            long popNext = pop * ( 1 + hab/100 );
            return modelGrowth(turns - 1, popNext, hab);
        }
    }

    public IVisiblePlanet placeToSend( ArrayList<IVisiblePlanet> planets ){
        // gets most habitable non full non opponent owned planet

        IVisiblePlanet best = bestLongRunPlanet( planets );

        if ( planets.size() == 1 ){
           return best;
        }
        else if ( almostFull( best ) ) {
           planets.remove(best);
           return placeToSend( planets );
        }
        else {
            return best;
        }
    }

    public boolean almostFull( IVisiblePlanet vp ){
        // checks if a planet is about to be full

        boolean b = false;
        if ( vp.getPopulation() > vp.getSize() - vp.getSize()/10 ){
            b = true;
        }
        return b;
    }

    public void storeInfo( HashMap<IVisiblePlanet, Integer> dict, IVisiblePlanet vp, Integer data){
        if ( !dict.containsKey(vp) ){
            dict.put(vp, data);
        }
    }

    public ArrayList<IVisiblePlanet> enemyPlanets( IPlanet vp, List<IPlanet> planets ){

        Queue<IPlanet> bft = new LinkedList<>();
        ArrayList<Integer> visited = new ArrayList<>();
        ArrayList<IVisiblePlanet> ret = new ArrayList<>();

        bft.add(vp);
        visited.add(vp.getId());

        while ( bft.peek() != null ){

            IPlanet temp = bft.remove();

            Iterator<IEdge> it = temp.getEdges().iterator();
            while ( it.hasNext() ){
                IEdge t2 = it.next();

                if ( !visited.contains( t2.getDestinationPlanetId() ) ) { // checks if the planet has been visited
                    visited.add(t2.getDestinationPlanetId());

                    IPlanet holder = getPlanet(t2.getDestinationPlanetId(), planets);

                    if ( holder instanceof IVisiblePlanet ) {

                        IVisiblePlanet da1 = (IVisiblePlanet) holder;
                        bft.add(da1);
                        if ( da1.getOwner() == Owner.OPPONENT ) {
                            ret.add(da1);
                        }
                    }
                }
            }
        }
        return ret;
    }

    public ArrayList<IVisiblePlanet> enemyOrNeutralPlanets( IVisiblePlanet vp, List<IPlanet> planets ){

        Queue<IVisiblePlanet> bft = new LinkedList<>();
        ArrayList<Integer> visited = new ArrayList<>();
        ArrayList<IVisiblePlanet> ret = new ArrayList<>();

        bft.add(vp);
        visited.add(vp.getId());

        while ( bft.peek() != null ){

            IVisiblePlanet temp = bft.remove();

            Iterator<IEdge> it = temp.getEdges().iterator();
            while ( it.hasNext() ){
                IEdge t2 = it.next();

                if ( !visited.contains( t2.getDestinationPlanetId() ) ) { // checks if the planet has been visited
                    visited.add(t2.getDestinationPlanetId());

                    IPlanet holder = getPlanet(t2.getDestinationPlanetId(), planets);

                    if (holder instanceof IVisiblePlanet) {

                        IVisiblePlanet da1 = (IVisiblePlanet) holder;
                        bft.add(da1);
                        if (da1.getOwner() == Owner.OPPONENT || da1.getOwner() == Owner.NEUTRAL) {
                            ret.add(da1);
                        }
                    }
                }
            }
        }
        return ret;
    }

//    public Stack<Integer> walmartDijkstra( IVisiblePlanet start, IVisiblePlanet finish, List<IPlanet> planets) {
//        // returns the next planet on the shortest path to the front lines along with the total distance away
//        PriorityQueue<BigDecimal> pq = new PriorityQueue<>(); // creates a priority queue of distances
//        Map<IVisiblePlanet, ArrayList<BigDecimal>> nodes = new HashMap<>();
//        ArrayList<IVisiblePlanet> visited = new ArrayList<>();
//        BigDecimal d = new BigDecimal(.0000000000001);
//
//        for (IPlanet p : planets) { /* sets up a dictionary with planets and their
//                                                            corresponding distances and previous planets */
//            ArrayList<BigDecimal> data = new ArrayList<>();
//            data.add( new BigDecimal(p.getId())); // adds previous planet
//            data.add( d ); // adds start distance which would be a minute identifier away from 0
//
//            d = d.add( new BigDecimal(.0000000000001));
//
//            nodes.put((IVisiblePlanet) p, data); // puts it on the map
//        }
//
//        ArrayList<BigDecimal> arr1 = new ArrayList<>();
//        arr1.add( new BigDecimal(0));
//        arr1.add( new BigDecimal(0));
//        nodes.replace(start, arr1); // sets the start planets distance of 0 and we can ignore its previous planet
//
//        pq.add(nodes.get(start).get(0)); // only have things in the queue that is visited
//
//        while ( getPlanetFromDict( nodes, pq.peek ()) != finish ) {
//
//            // this gets the planet with the least distance form the origin via the priority queue
//            IVisiblePlanet vp = getPlanetFromDict( nodes, pq.poll() );
//
//            visited.add(vp); // keeps track of the visited planets
//
//            Iterator<IEdge> it = vp.getEdges().iterator();
//
//            while ( it.hasNext() ) {
//
//                IEdge edge = it.next();
//                IVisiblePlanet vp2 = getPlanet(edge.getDestinationPlanetId(), planets); // gets next planet
//
//                if ( !visited.contains(vp2) ) { // checks if the planet has already been visited
//
//                    ArrayList<BigDecimal> arr2 = new ArrayList<>();
//                    arr2.add( new BigDecimal( vp.getId()) ); // adds previous planet id
//
//                    BigDecimal newDist = nodes.get(vp2).get(1).add( new BigDecimal(edge.getLength())); // calculates new distance
//
//                    arr2.add(newDist); // updates the distance
//                    nodes.replace(vp2, arr2); // replaces the updated distance and previous planet
//
//                    pq.add(newDist); // adds the new distance to the priority queue
//                }
//            }
//        }
//
//        // need to think about the looping path case
//
//        Stack<Integer> ret = new Stack<>();
//        BigDecimal value = pq.poll();
//        IVisiblePlanet next = getPlanetFromDict( nodes, value );
//        int daDist = value.intValue();
//
//        IVisiblePlanet vp = next,vpb4 = next;
//        while ( vp != start ){ // traces the planets back to the beginning
//            vpb4 = vp;
//            BigDecimal bd = nodes.get(vp).get(0);
//            int id = bd.intValue();
//            vp = getPlanet( id, planets);
//        }
//
//        ret.push(vpb4.getId());
//        ret.push(daDist);
//
//        return ret;
//    }

    public Stack<Integer> walmartDijkstra2( IPlanet start, IPlanet finish, List<IPlanet> planets) {
        ArrayList<vertex> comparing = new ArrayList<>();
        ArrayList<IPlanet> visited = new ArrayList<>();

        vertex begin = new vertex(start, null, 0);
        comparing.add(begin);

        while ( planetFromVertexArrray( comparing, finish ) != finish ){
            // checks if the destination planet has been added to the list in a vertex

            vertex holder = findMin4u( comparing ); // gets vertex with min distance from origin
            IPlanet vp = holder.current; // enables neighbor getting
            visited.add(vp); // keeps track of visited planets
            Iterator<IEdge> it = vp.getEdges().iterator();

            while ( it.hasNext() ) {

                IEdge edge = it.next();
                IPlanet vp2 = getPlanet(edge.getDestinationPlanetId(), planets); // gets next planet

                if (!visited.contains(vp2)) {
                    vertex v = new vertex(vp2, vp, holder.dist + edge.getLength());
                    comparing.add(v);
                }
            }
        }
        Stack<Integer> ret = new Stack<>();
        vertex v = vertexFromPlanet( comparing, finish );
        int dist = v.dist;
        IPlanet vp = v.current,vpb4 = vp;

        while ( vp != start){
            vpb4 = vp;
            vp = v.prev;
            v = vertexFromPlanet( comparing, vp );
        }

        ret.push(vpb4.getId());
        ret.push(dist);

        return ret;
    }

    public vertex findMin4u( ArrayList<vertex> comparing ){
        vertex da1 = comparing.get(0);
        for ( vertex v : comparing ) {
            if ( v.dist < da1.dist ){
                da1 = v;
            }
        }
        return da1;
    }

    public IPlanet planetFromVertexArrray( ArrayList<vertex> comparing, IPlanet finish ){
        IPlanet da1 = comparing.get(0).current;
        for ( vertex v : comparing ) {
            if ( v.current == finish ){
                da1 = v.current;
            }
        }
        return da1;
    }

    public vertex vertexFromPlanet( ArrayList<vertex> comparing, IPlanet vp ){
        vertex V = comparing.get(0);
        for ( vertex v : comparing ) {
            if ( v.current == vp ){
                V = v;
            }
        }
        return V;
    }

    public IVisiblePlanet toTheFront( IVisiblePlanet vp , List<IPlanet> planets ){
        ArrayList<IVisiblePlanet> enemies = enemyOrNeutralPlanets( vp , planets);
        ArrayList<Stack<Integer>> shortestPaths = new ArrayList<>();

        for ( IVisiblePlanet p : enemies ) {
            shortestPaths.add(walmartDijkstra2( vp, p, planets ));
        }

        Stack<Integer> path = shortestPaths.get(0);
        int min = path.peek();

        for ( Stack<Integer> stack : shortestPaths ) {
            int compare = stack.pop();
            if ( min > compare){
                path = stack;
            }
        }

        return (IVisiblePlanet) getPlanet( path.pop(), planets );
    }

    public int longRunScore( IVisiblePlanet vp ){
        // quantifies how fast a planet will generate people at its capacity

        int hab = vp.getHabitability();
        int size = (int) vp.getSize();
        int value = size + hab/100;

        return value;
    }

    public IVisiblePlanet bestLongRunPlanet( ArrayList<IVisiblePlanet> planets ){

        IVisiblePlanet best = planets.get(0);

        Map<IVisiblePlanet, Integer> compare = new HashMap<>();

        for ( IVisiblePlanet p : planets ) {

            int value = longRunScore( p );
            compare.put(p, value);
        }

        for ( IVisiblePlanet entry : planets ) {
            int yolo = compare.get(entry);
            int olga = compare.get(best);
            if ( yolo > olga){
                best = entry;
            }
        }
        return best;
    }

    public IPlanet getPlanet( int i, List<IPlanet> planets ){

        return planets.get(i);
    }

    public IVisiblePlanet getPlanetFromDict( Map<IVisiblePlanet, ArrayList<BigDecimal>> nodes, BigDecimal distance ){
        /* what if two planets are the same distance? it will return the first one which may not be the right one
         *
         * this is solved by adding a minute unique BigDecimal to each distance value in the dictionary
         * that would differentiate distances by a small enough amount to where it wouldn't matter */

        for ( Map.Entry<IVisiblePlanet, ArrayList<BigDecimal>> entry : nodes.entrySet() ) {
            IVisiblePlanet vp = entry.getKey();
            if ( entry.getValue().get(1).equals(distance) ){
                return vp;
            }
        }
        return null;
    }

    public int numNeighborNeutral( ArrayList<IVisiblePlanet> neighbors ){
        // makes sure that a planet doesn't send more ppl away than it owns
        int numNuetral = 0;
        for ( IVisiblePlanet p : neighbors ) {
            if ( p.getOwner() == Owner.NEUTRAL ){
                numNuetral++;
            }
        }
        return numNuetral;
    }

    public class vertex {
        IPlanet current, prev;
        int dist;
        vertex( IPlanet current, IPlanet prev, int distance) {
            this.current = current;
            this.prev = prev;
            dist = distance;
        }

        public int getDist() {
            return dist;
        }

        public IPlanet getCurrent() {
            return current;
        }

        public IPlanet getPrev() {
            return prev;
        }

        public void setCurrent(IPlanet current) {
            this.current = current;
        }

        public void setDist(int dist) {
            this.dist = dist;
        }

        public void setPrev(IPlanet prev) {
            this.prev = prev;
        }
    }

    public Graph<IPlanet, IEdge> edgeEdgeandEdgey( List<IPlanet> planets ){

        Graph<IPlanet, IEdge> graph = new SimpleWeightedGraph<>(IEdge.class);

        System.out.println("Passed graph instantiation");

        int count = 0;
        for ( IPlanet p : planets ) {

            if ( count == 0 ){
                graph.addVertex(p);
                count++;
            }

            Iterator<IEdge> it = p.getEdges().iterator();

            while ( it.hasNext() ){

                IEdge edge = it.next();
                IPlanet ip = getPlanet( edge.getDestinationPlanetId(), planets );

                if ( !graph.containsVertex(ip) ){

                    graph.addVertex( ip );
                    graph.addEdge( p, ip, edge );
                }
            }
        }
        return graph;
    }

    public IVisiblePlanet nextP( IVisiblePlanet vp, List<IPlanet> planets ){
        Graph<IPlanet, IEdge> g = edgeEdgeandEdgey( planets );
        ArrayList<IVisiblePlanet> enemies = enemyOrNeutralPlanets( vp, planets );
        Map<GraphPath<IPlanet, IEdge>, List<IPlanet>> datMap = new HashMap<>();

        for ( IVisiblePlanet p : enemies ) {
            GraphPath<IPlanet, IEdge> path = org.jgrapht.alg.shortestpath.DijkstraShortestPath.findPathBetween( g, vp, p );
            List<IPlanet> planetPath = path.getVertexList();
            datMap.put(path, planetPath);
        }

        Iterator< GraphPath<IPlanet, IEdge> > minPaths = datMap.keySet().iterator();
        GraphPath<IPlanet, IEdge> minPath = minPaths.next();
        List<IPlanet> Neo = datMap.get(minPath);

        for ( Map.Entry<GraphPath<IPlanet, IEdge>, List<IPlanet>> entry : datMap.entrySet() ) {

            GraphPath<IPlanet, IEdge> tempPath = entry.getKey();
            List<IPlanet> tempList = entry.getValue();

            if ( tempPath.getLength() < minPath.getLength() ){
                Neo = tempList; // changes the min path for the list of planets
            }
        }
        return (IVisiblePlanet) Neo.get(1);
    }

    public boolean compete() {
        return true;
    }

    public String getName() { return "Cardam"; }
}
