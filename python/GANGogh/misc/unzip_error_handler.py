# import zipfile, os
#
# with zipfile.ZipFile('images.zip', 'r') as zip_ref:
#     # zip_ref.extractall("images")
#     # print(len(os.getcwd()))
#     toolongs = 0
#     notfounds = 0
#     for o in zip_ref.infolist():
#         # print(o.filename)
#         try:
#             if not (len(o.filename) + len(os.getcwd()) > 260):
#                 zip_ref.extract(o)
#             else:
#                 idx = o.filename.rfind("/")
#                 o.filename = o.filename[:idx] + "/too_long" + str(toolongs)
#                 toolongs += 1
#                 zip_ref.extract(o)
#                 print(toolongs)
#         except(FileNotFoundError):
#             print("{} not found. Happened {} times so far".format(o.filename, notfounds))
# import scipy.misc
# image = scipy.misc.imread(os.path.normpath("C:\\Users\\A-dizzle\\PycharmProjects\\GANs\\GANGogh\\small_images\\abstract\\0.png"))