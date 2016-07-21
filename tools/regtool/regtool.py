#!/usr/bin/env python

# This program converts a discription of registers in xml format
# into a c++ header file
# usage = 'Usage: ./regtool.py -i <filename> -o <header_filename> -p <project_name> -n <namespace>\n'

import sys, getopt
import xml.etree.ElementTree as element_tree

def main(argv):
   try:
      opts, args = getopt.getopt(argv[1:], 'i:o:p:n:')
   except getopt.GetoptError as err:
      sys.exit(2)

   xmlfile = 'Registers.xml'
   header = 'Registers.hpp'
   project = ''
   namespace = ''
   for opt, arg in opts:
      if opt == "-i":
         xmlfile = arg
      elif opt == "-o":
         header = arg
      elif opt == "-p":
         project = arg
      elif opt == "-n":
         namespace = arg  

   tree = element_tree.parse(xmlfile)
   root = tree.getroot()

   f = open(header, 'w');

   guard = (project.upper() + "_" + header.split('/')[-1].upper() + "_").replace('.', '_')        

   f.write('#ifndef ' + guard + '\n')
   f.write('#define ' + guard + '\n')

   if namespace:
      f.write('\nnamespace ' + namespace + '\n{\n')

   f.write('\nenum \n{\n')

   for child in root[:-1]:
      equality = "   {0} = {1}".format(child.tag, child.attrib['address'])
      print(equality)
      f.write(equality + ", // {0}\n".format(child.attrib['mode']))

   equality = "   {0} = {1}".format(root[-1].tag, root[-1].attrib['address'])
   f.write(equality + "// {0}\n}}; // enum\n".format(root[-1].attrib['mode']))  

   if namespace:
      f.write('\n} // namespace ' + namespace + '\n')

   f.write('\n#endif // ' + guard + '\n')  
   f.close()

   print(header + " has been successfully created")

if __name__ == "__main__":
   main(sys.argv)

