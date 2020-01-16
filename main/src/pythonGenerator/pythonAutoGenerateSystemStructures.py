# -*- coding: utf-8 -*-
"""
Created on Fri May 18 08:53:30 2018

@author: Johannes Gerstmayr

goal: automatically generate interfaces for structures
currently: automatic generate structures with ostream and initialization
"""
import datetime # for current date
from autoGenerateHelper import RemoveSpacesTabs, CountLines, TypeConversion, GenerateHeader, SplitString, Str2Latex, Str2Doxygen, GetDateStr


typeCasts = {'Bool':'bool', 'Int':'int', 'Real':'Real', 'UInt':'Index', 'UReal':'Real', 
             'Vector':'std::vector<Real>', 'Vector3D':'std::vector<Real>', 'Matrix':'Matrix', 'SymmetricMatrix':'Matrix', 
             'ArrayIndex':'std::vector<Index>', 'String':'std::string',
             'Float2': 'std::array<float,2>', 'Float3': 'std::array<float,3>', 'Float4': 'std::array<float,4>',  #e.g. for OpenGL vectors
             'Float9': 'std::array<float,9>', 'Float16': 'std::array<float,16>', #e.g. for OpenGL rotation matrix and homogenous transformation
             'Index2': 'std::array<Index,2>', 'Index3': 'std::array<Index,3>', 'Index4': 'std::array<Index,4>', 
             } #convert parameter types to C++/DYNALFEX types


#************************************************
#create autogenerated .h  file for list of parameters
def WriteFile(parseInfo, parameterList, typeConversion):
    #print (parseInfo)
    #print('file="'+parseInfo['writeFile']+'"')
    
    now=datetime.datetime.now()
    monthZero = '' #add leading zero for month
    dayZero = ''   #add leading zero for day
    if now.month < 10:
        monthZero = '0'
    if now.day < 10:
        dayZero = '0'
        
    dateStr = str(now.year) + '-' + monthZero + str(now.month) + '-' + dayZero + str(now.day)
    
    sLatex = parseInfo['latexText'] #.replace('\\n','\n') #this is the string for latex documentation
    #************************************
    #header
    s='' #generate a string for the output file
    #s+='//automatically generated file (pythonAutoGenerateInterfaces.py)\n'
    s+='/** ***********************************************************************************************\n'
    s+='* @class        '+parseInfo['class']+'\n'
    s+='* @brief        '+Str2Doxygen(parseInfo['classDescription'])+'\n'
    s+='*\n'
    s+='* @author       AUTO: Gerstmayr Johannes\n'
    s+='* @date         AUTO: 2019-07-01 (generated)\n'
    s+='* @date         AUTO: '+ dateStr+' (last modfied)\n'
    s+='*\n'
    s+='* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.\n'
    s+='* @note         Bug reports, support and further information:\n'
    s+='                - email: johannes.gerstmayr@uibk.ac.at\n'
    s+='                - weblink: missing\n'
    s+='                \n'
    s+='************************************************************************************************ **/\n'
    s+='#pragma once\n'
    s+='\n'
    s+='#include <ostream>\n'
    s+='\n'
    s+='#include "Utilities/ReleaseAssert.h"\n'
    s+='#include "Utilities/BasicDefinitions.h"\n'
    s+='#include "Main/OutputVariable.h"\n'
    s+='#include "Linalg/BasicLinalg.h"\n' #for std::array conversion
    s+='\n'

    #************************************
    #Latex doc:
    hasPybindInterface = False
    for parameter in parameterList:
        if (parameter['lineType'].find('V') != -1) and (parameter['cFlags'].find('P') != -1): #only if it is a member variable
            hasPybindInterface = True

    if hasPybindInterface: #otherwise do not include the description into latex doc
        descriptionStr = parseInfo['classDescription']
        if descriptionStr[-1] != '.': descriptionStr += '. '
        
        sLatex += '\n%+++++++++++++++++++++++++++++++++++\n\mysubsubsection{' + parseInfo['class'] + '} \label{sec_' + parseInfo['class'].replace(' ','_') + '}\n'
        sLatex += Str2Latex(descriptionStr) + '\\\\ \n'
        sLatex += '%\n'
        sLatex += parseInfo['class'] + ' has the following items:\n'
        sLatex += '%reference manual TABLE\n'
        sLatex += '\\begin{center}\n'
        sLatex += '  \\footnotesize\n'
        sLatex += '  \\begin{longtable}{| p{4.2cm} | p{2.5cm} | p{0.3cm} | p{3.0cm} | p{6cm} |}\n'
        sLatex += '    \\hline\n'
        sLatex += '    \\bf Name & \\bf type / function return type & \\bf size & \\bf default value / function args & \\bf description \\\\ \\hline\n'
    
        for parameter in parameterList:
            if (parameter['lineType'].find('V') != -1) and (parameter['cFlags'].find('P') != -1): #only if it is a member variable
                sString = ''
                if (parameter['type'] == 'String'):
                    sString="'"
                #write latex doc:
                defaultValueStr = parameter['defaultValue']
                paramDescriptionStr = parameter['parameterDescription']
                if len(defaultValueStr) > 18:
                    paramDescriptionStr = '\\tabnewline ' + paramDescriptionStr
                pythonName = Str2Latex(parameter['pythonName']) 
                typeName = Str2Latex(parameter['type'])
                
                if len(pythonName)>28: 
                    typeName = '\\tabnewline ' + typeName
                    
                if parameter['type'] != 'String':
                    defaultValueStr = Str2Latex(defaultValueStr, True)
                    
                sLatex += '    ' + pythonName + ' & '                
                sLatex += '    ' + typeName + ' & '
                sLatex += '    ' + Str2Latex(parameter['size']) + ' & '
                sLatex += '    ' + sString+defaultValueStr+sString + ' & '
                sLatex += '    ' + paramDescriptionStr + '\\\\ \\hline\n' #Str2Latex not used, must be latex compatible!!!

            if (parameter['lineType'].find('F') != -1) and (parameter['cFlags'].find('P') != -1): #only if it is a function
                #write latex doc:
                functionName = Str2Latex(parameter['pythonName'])
                argStr = parameter['args']
                if (argStr != ''):
                    functionName += '(...)'
                    argSplit = argStr.split(',') #split into list of args
                    argStr = ''
                    argSep = '' #no comma for first time
                    for item in argSplit:
                        argName = item.split(' ')[-1] #last word in args is the name of the argument, e.g. in const MainSystem& mainSystem ==> mainSystem
                        argName = Str2Latex(argName)
                        argStr += argSep + argName
                        argSep = ', '
                else: 
                    functionName += '()'

                functionType = Str2Latex(parameter['type'])
                if (len(functionName)>28): 
                    functionType = '\\tabnewline ' + functionType

                sLatex += '    ' + functionName + ' & '
                sLatex += '    ' + functionType + ' & '
                sLatex += '    ' + Str2Latex(parameter['size']) + ' & '

                sLatex += '    ' + argStr + ' & '
                sLatex += '    ' + Str2Latex(parameter['parameterDescription']) + '\\\\ \\hline\n' #Str2Latex not used, must be latex compatible!!!
                
        sLatex += '	  \\end{longtable}\n'
        sLatex += '	\\end{center}\n'


    #************************************
    #class definition:
    strParentClass = ''
    if len(parseInfo['parentClass']) != 0:
        strParentClass = ': public ' + parseInfo['parentClass']
    s+='class ' + parseInfo['class'] + strParentClass + ' // AUTO: \n'
    s+='{\n'

    #************************************
    #member variables:
    sPublic = ''
    sPrivate = ''
    sProtected = ''
    for parameter in parameterList:
        if (parameter['lineType'].find('V') != -1) and (parameter['lineType'].find('L') == -1) and (parameter['cplusplusName'].find('.') == -1): #only if it is a member variable, but not linked
            typeStr = TypeConversion(parameter['type'], typeConversion)
            temp = '  ' + typeStr + ' ' + parameter['cplusplusName']+ ';'
            nChar = len(temp)
            alignment = 50
            insertSpaces = ''
            if nChar < alignment:
                insertSpaces = ' '*(alignment-nChar)
            temp += insertSpaces + '//!< AUTO: ' + Str2Doxygen(parameter['parameterDescription']) + '\n'

            if (parameter['lineType'].find('p') != -1): #make variable private ==> no direct access via C++ or python!
                sPrivate += temp
            else:
                if (parameter['cFlags'].find('P') != -1): #pybind of member variables in this case done via public member variable
                    sPublic += temp
                else:
                    sProtected += temp
                

    if (sPublic !='' or sProtected !=''):
        s+='public: // AUTO: \n'
        s+=sPublic #public member variables
        s+=sProtected #protected member variables
        s+='\n'
#    if (sProtected !=''):
#        s+='protected: // AUTO: \n'
#        s+=sProtected #protected member variables
#        s+='\n'
    if (sPrivate !=''):
        s+='private: // AUTO: \n'
        s+=sPrivate #private member variables
        s+='\n'
            
    s+='\npublic: // AUTO: \n' #for member functions ...
    #************************************
    #count number of default parameters
    cntDefaultParameters = 0
    for parameter in parameterList:
        if (parameter['lineType'].find('V') != -1): #only if it is a member variable
            strDefault = parameter['defaultValue']
            if len(strDefault): 
                cntDefaultParameters += 1

    #constructor with default initialization:
    if cntDefaultParameters or len(parseInfo['addConstructor']) != 0:
        s+='  //! AUTO: default constructor with parameter initialization\n'
        s+='  '+parseInfo['class']+'()\n'
        s+='  {\n'
    
        for parameter in parameterList:
            if (parameter['lineType'].find('V') != -1): #only if it is a member variable
                strDefault = parameter['defaultValue']
                if len(strDefault): #only add initialization if default value exists
                    if parameter['type'] == 'String':
                        strDefault = '"' + strDefault + '"'
                    s+='    ' + parameter['cplusplusName'] + ' = ' + strDefault + ';\n'
        s+=parseInfo['addConstructor'].replace('\\n','\n')
        s+='  };\n'

    s+='\n  // AUTO: access functions\n'

    #GetClone() function: #2020-01-03: not used any more
#    s+='  //! AUTO: clone object; specifically for copying instances of derived class, for automatic memory management e.g. in ObjectContainer\n'
#    s+='  virtual ' + parseInfo['class'] + '* GetClone() const { return new '+parseInfo['class']+'(*this); }\n'
#    s+='  \n'

    #************************************
    #access functions:
    for parameter in parameterList:
        if (parameter['lineType'].find('V') != -1): #only if it is a member variable
            typeStr = TypeConversion(parameter['type'], typeConversion)
            paramStr = parameter['cplusplusName']
            paramStrPure = parameter['cplusplusName'] #without 'cSolver.'
            if (paramStr.find('.') != -1): #for linked class
                paramStrPure = parameter['pythonName']

            functionStr = paramStrPure
            c = functionStr[0]
            functionStr = c.upper()+functionStr[1:]
            refChar = '&' #use only '&' in read access, if it is no pointer; 
            if typeStr[len(typeStr)-1] == '*':
                refChar = ''
    
            accessWritten = False
            if parameter['cFlags'].find('A') != -1:
                accessWritten = True
                s+='  //! AUTO: Read (Reference) access to: ' + Str2Doxygen(parameter['parameterDescription']) + '\n'
                s+='  const ' + typeStr + refChar + ' '
                s+='Get' + functionStr + '() const { return '+paramStr+'; }\n'
    
                s+='  //! AUTO: Write (Reference) access to: ' + Str2Doxygen(parameter['parameterDescription']) + '\n'
                s+='  '+typeStr + '&' + ' '
                s+='Get' + functionStr + '() { return ' + paramStr + '; }\n'

            typeCastStr = TypeConversion(parameter['type'], typeCasts)
            if (typeCastStr.find('std::vector') != -1 or typeCastStr.find('std::array') != -1) or (parameter['lineType'].find('L') == -1  and parameter['cplusplusName'].find('.') != -1): #then it must get a set/get function!
                accessWritten = True
                castStr = '(' + typeCastStr + ')'
                linkedClassStr = ''
#                if (len(parseInfo['linkedClass']) != 0) and (parameter['lineType'].find('L') != -1):
#                    linkedClassStr = parseInfo['linkedClass'] + '.'
                
                s+='  //! AUTO: Set function (needed in pybind) for: ' + Str2Doxygen(parameter['parameterDescription']) + '\n'
                s+='  void '
                s+='PySet' + functionStr + '(const ' + typeCastStr + refChar + ' ' + paramStrPure + 'Init) { ' + linkedClassStr + paramStr + ' = ' + paramStrPure + 'Init; }\n'

                s+='  //! AUTO: Read (Copy) access to: ' + Str2Doxygen(parameter['parameterDescription']) + '\n'
                s+='  '+typeCastStr + ' '
                s+='PyGet' + functionStr + '() const { return ' + castStr + '('+linkedClassStr + paramStr + '); }\n'
            
            if accessWritten:
                s+= '\n'

    
        else: # must be member function
            if parameter['lineType'].find('L') == -1:
                strVirtual = ''
                strOverride = ''
                if (parameter['lineType'].find('v') != -1):
                    strVirtual = 'virtual '
                    strOverride = ' override'
                
                typeStr = TypeConversion(parameter['type'], typeConversion)
                functionStr = parameter['cplusplusName']
                argsStr = parameter['args']
                strConst = ""
                if parameter['cFlags'].find('C') != -1:
                    strConst = " const"
                
                strDef = ''            
                if parameter['cFlags'].find('D') != -1:
                    strDef = ';'
                else:
                    strDef = ' {\n    ' + parameter['defaultValue'] + '\n  }\n' #defaultValue is the function body
    
                s+='  //! AUTO: ' + Str2Doxygen(parameter['parameterDescription']) + '\n'
                s+='  '+strVirtual + typeStr + ' '
                s+=functionStr + '(' + argsStr + ')' + strConst + strOverride + strDef + '\n'
    
        

    #************************************
    #ostream operator:
#    s+=('  friend std::ostream& operator<<(std::ostream& os, const ' + 
#       parseInfo['class'] + '& object);\n')

    s+='  //! AUTO: print function used in ostream operator (print is virtual and can thus be overloaded)\n'
    s+='  virtual void Print(std::ostream& os) const\n'
    s+='  {\n'
    s+='    os << "' + parseInfo['class'] + '" << ":\\n";\n'
    if len(parseInfo['parentClass']) != 0:
        s+='    os << ":"; \n'
        s+='    ' + parseInfo['parentClass'] + '::Print(os);\n'
        
    #output each parameter
    for parameter in parameterList:
        if (parameter['lineType'].find('V') != -1) and (parameter['lineType'].find('L') == -1) and (parameter['type']!='TemporaryComputationData') and (parameter['type']!='std::ofstream') and (parameter['type'].find('userFunction')==-1): #only if it is a member variable; some types not printable
            paramStr = parameter['cplusplusName']
            typeStr = TypeConversion(parameter['type'], typeConversion)
            refChar = ''
            preStr = ''
            postStr = ''
            if typeStr[len(typeStr)-1] == '*':
                refChar = '*' #print content of object, not the pointer address
            if parameter['type'] == 'OutputVariableType': #special case of enum, which is not printable
                preStr = 'GetOutputVariableTypeString('
                postStr= ')'
            if parameter['type'] == 'StdArray33F': #special case of enum, which is not printable
                preStr = 'Matrix3DF('
                postStr= ')'
            s+='    os << "  ' + paramStr + ' = " << ' + preStr + refChar + paramStr + postStr + ' << "\\n";\n'
         
    s+='    os << "\\n";\n'
    s+='  }\n\n' # end ostream operator

    if len(parseInfo['parentClass']) == 0:
        s+=('  friend std::ostream& operator<<(std::ostream& os, const ' + parseInfo['class'] + '& object)\n')
        s+= '  {\n'
        s+= '    object.Print(os);\n'
        s+= '    return os;\n'
        s+= '  }\n\n' # end ostream operator
    
    s+='};\n\n\n' #class


    return [s, sLatex]

#**************************************************************************************
#**************************************************************************************
#**************************************************************************************
#create string containing the pybind11 headers/modules for a class
def CreatePybindHeaders(parseInfo, parameterList, typeConversion):
    #print ('Create Pybind11 includes')

    spaces1 = '    '            #first level
    spaces2 = spaces1+'    '    #second level

    s = spaces1 + '//++++++++++++++++++++++++++++++++\n' #create empty string
    #************************************
    #class definition:
    parentClass = ''
#    if len(parseInfo['parentClass']) != 0: #derived class does not work in pybind, if parent class is not defined!
#        parentClass = ', ' + parseInfo['parentClass']
    s += spaces1 + 'py::class_<' + parseInfo['class'] + parentClass + '>(m, "' + parseInfo['class'] + '") // AUTO: \n'
    s += spaces2 + '.def(py::init<>())\n'
	
    #************************************
    #member variables access:
    for parameter in parameterList:
        if (parameter['lineType'].find('V') != -1) and (parameter['cFlags'].find('P') != -1): #only if it is a member variable
            typeCastStr = TypeConversion(parameter['type'], typeCasts)
            linkedClassStr = ''
            if (len(parseInfo['linkedClass']) != 0):
                linkedClassStr = parseInfo['linkedClass'] + '.'

            if (typeCastStr.find('std::vector') == -1) and (typeCastStr.find('std::array') == -1) and (parameter['lineType'].find('L') == -1) and (parameter['cplusplusName'].find('.') == -1): #then it has a set/get function! e.g. Int2, Int3, Float2, Float3, .... are array structures ==> must be converted
                s += spaces2 + '.def_readwrite("' + parameter['pythonName'] + '", &' + parseInfo['class'] + '::' + linkedClassStr + parameter['cplusplusName'] + ')\n' #extend this to incorporate 'read only' and other flags
            else:
                #access with setter/getter functions and conversions to std::vector
                #functionName = parameter['cplusplusName']
                functionName = parameter['pythonName'] #for linked variables, this is easier to work with linking e.g. to cSolver
                functionName = functionName[0].upper()+functionName[1:]
                s += spaces2 + '.def_property("' + parameter['pythonName'] + '", &' + parseInfo['class'] + '::PyGet' + functionName
                s += ', &' + parseInfo['class'] + '::PySet' + functionName + ')\n'
                #    .def_property("name", &Pet::getName, &Pet::setName)
                
    #s += '\n'
    s += spaces2 + '// AUTO: access functions for ' + parseInfo['class'] + '\n'
            
    for parameter in parameterList:
        if (parameter['lineType'].find('F') != -1) and (parameter['cFlags'].find('P') != -1): #only if it is a member function
            s += spaces2 + '.def("' + parameter['pythonName']
            s += '", &' + parseInfo['class'] + '::' + parameter['pythonName']
            if parameter['type'] != 'void': #check return_value_policy if not void
                if parameter['cFlags'].find('V') != -1: #pass by value (copy)
                    s += ', py::return_value_policy::copy'
                else:
                    s += ', py::return_value_policy::reference' #extend this to incorporate 'read only' and other flags
            s += ', "' + parameter['parameterDescription'] + '"'
            if (parameter['cFlags'].find('G') != -1): #add py::arg() in order that type completion shows args in python
                argStr = parameter['args']
                if (argStr != ''):
                    argSplit = argStr.split(',') #split into list of args
                    for item in argSplit:
                        argName = item.split(' ')[-1] #last word in args is the name of the argument, e.g. in const MainSystem& mainSystem ==> mainSystem
                        defaultVal = ''

                        if item.find('=') != -1: #check if there is a default value
                            itemSplit = item.split('=')
                            argName = itemSplit[0].split(' ')[-1] #in left structure, there must be the argument name
                            #print(itemSplit)
                            defaultVal = ' = ' + itemSplit[1]
                        s += ', py::arg("' + argName + '")' + defaultVal
            s+=')\n' 
    
    s += spaces2 + '.def("__repr__", [](const ' + parseInfo['class'] + ' &item) { return "<' + parseInfo['class'] + ':\\n" + EXUstd::ToString(item) + " >"; } ) //!< AUTO: add representation for object based on ostream operator\n'

    s += spaces2 + '; // AUTO: end of class definition!!!\n'
    s += '\n'

    s += spaces1 + '//++++++++++++++++++++++++++++++++\n' #end of pybind11 definition


    return s



#************************************************
#MAIN CONVERSION
#************************************************
    

try: #still close file if crashes
    #create Python/pybind11 file
    directoryString = '..\\Autogenerated\\'
    pybindFile = directoryString+'pybind_modules.h'
    latexFile = '..\\..\\..\\docs\\theDoc\\interfaces.tex'
    file=open(pybindFile,'w')  #clear file by one write access
    file.write('// AUTO:  ++++++++++++++++++++++\n')
    file.write('// AUTO:  pybind11 module includes; generated by Johannes Gerstmayr\n')
    file.write('// AUTO:  last modified = '+ GetDateStr() + '\n')
    file.write('// AUTO:  ++++++++++++++++++++++\n')
    file.close()
    
    #read system definition
    filename = "systemStructuresDefinition.py"
    totalNumberOfLines = 0

    file=open(filename,'r') 
    
    fileLines = file.readlines()
    
    typeConversion = {'Bool':'bool', 'Int':'int', 'Real':'Real', 'UInt':'Index', 'UReal':'Real', 'Vector':'Vector', 
                      'Matrix':'Matrix', 'SymmetricMatrix':'Vector', 
                      'NumpyMatrix':'py::array_t<Real>', 'NumpyVector':'py::array_t<Real>', 
                      'String':'std::string'} #convert parameter types to C++/DYNALFEX types

    parseInfo = {'class':'',            # C++ class name
                 'writeFile':'',        #filename (e.g. SensorData.h)
                 'appendToFile':'',     #True, if shall be appended to given file
                 'writePybindIncludes':'',#True, if pybind11 includes shall be written for this class
                 'parentClass':'',      #name of parent class or empty
                 'classDescription':'', #add a (brief, one line) description of class
                 'addConstructor':'',   #code added at the end of default constructor
                 'linkedClass':'',      #if not empty, this is a class member to which the python interface is linked
                 'latexText':''}        #text, which will be added before the class description (e.g., to start a new section)
    lineDefinition = ['lineType',       #[V|F[v]]P: V...Value (=member variable), F...Function (access via member function); v ... virtual Function; P ... write Pybind11 interface
                      'pythonName',     #name which is used in python
                      'cplusplusName',     #name which is used in DYNALFEX (leave empty if it is the same)
                      'size',           #leave empty if size is variable; e.g. 3 (size of vector), 2x3 (2 rows, 3 columns)  %used for vectors and matrices only!
                      'type',           #Bool, Int, Real, UInt, UReal, Vector, Matrix, SymmetricMatrix
                      'defaultValue',   #default value for member variable or function definition
                      'args',           #args for functions
                      'cFlags',         #P(add Pybind11 interface), R(read only), M(modifiableDuringSimulation), C...const function, D...definition only [default is read/write access and that changes are immediately applied and need no reset of the system]
                      'parameterDescription'] #description for parameter used in C++ code
    nparam = len(lineDefinition)
    
    
    mode = 0 #1...read parameterlist , 0...read definitions
    linecnt = 1
    
    parameterList = [] #list of dictionaries for parameters
    continueOperation = True #flag to signal that operation shall be terminated
    
    globalLatexStr = '' #this is the whole string for the latex docu
    
    for line in fileLines:
        if continueOperation:
            if line[0] != '#':
                pureline = (line.strip('\n')) #eliminate EOL
                if len(pureline.replace(' ','')): #empty lines are ignored
                    if (pureline[0] == 'V') or (pureline[0] == 'F'):
                        #must be definition of parameters
                        info = SplitString(pureline, linecnt)
                        #print(info)
                        if len(info) != len(lineDefinition):
                            continueOperation = False
                        d={} #empty dictionary
                        
                        cnt = 0
                        for item in lineDefinition:
                            d[lineDefinition[cnt]] = info[cnt]
                            cnt+=1
                        if len(d['cplusplusName']) == 0:
                            d['cplusplusName'] = d['pythonName']
                        #print(d)
                        parameterList.append(d) #append parameter dictionary to list
                    elif (pureline.find('=') != -1): #definition
    #                    pureline = pureline.replace(' ','') #eliminate spaces and EOL
                        info = pureline.split('=', maxsplit = 1)
                        #print("info =", info)
                        if (len(info) == 2):
                            defName = info[0].replace(' ','')
                            #print("defname =",defName)
                            RHS = RemoveSpacesTabs(info[1])
                            RHS = RHS.strip('"')
                            if (defName != 'classDescription' and defName != 'latexText' and defName != 'addConstructor'):
                                RHS = RHS.replace(' ','')
                            if (defName == 'classDescription' or defName == 'latexText'):
                                RHS = RHS.replace('\\n','\n') #enable line breaks!
                                
                            if (defName in parseInfo):
                                parseInfo[defName] = RHS
                                #print(parseinfo)
                            else:
                                print("ERROR: invalid specifier", defName, "in line",linecnt)    
                                continueOperation = False
                            if (defName == "class"):
                                if (mode == 0):
                                    mode = 1
                                else:
                                    print("ERROR: did not expect 'class' keyword in line",linecnt)    
                                    continueOperation = False                       
                            if (defName == "writeFile"):
                                if (mode == 1):
                                    mode = 0
                                    #++++++++++++++++++++++++++++++
                                    #now write C++ header file for defined class
                                    [fileStr, latexStr] = WriteFile(parseInfo, parameterList, typeConversion)
                                    globalLatexStr += latexStr
                                    strFileMode = 'w'
                                    if parseInfo['appendToFile'] == 'True':
                                        strFileMode = 'a'
                                    file=open(directoryString+parseInfo['writeFile'],strFileMode) 
                                    file.write(fileStr)
                                    file.close()

                                    #++++++++++++++++++++++++++++++
                                    #write Python/pybind11 includes
                                    if parseInfo['writePybindIncludes'] == 'True':
                                        pybindStr = CreatePybindHeaders(parseInfo, parameterList, typeConversion)
                                        file=open(pybindFile,'a')  #always append to pybind file
                                        file.write(pybindStr)
                                        file.close()
                                    
                                    #++++++++++++++++++++++++++++++
                                    #reset structures for next file
                                    totalNumberOfLines += CountLines(fileStr)
                                    parameterList = [] #reset list
                                    parseInfo['appendToFile'] = 'False'
                                    parseInfo['writeFile'] = ''
                                    parseInfo['class'] = ''
                                    parseInfo['parentClass'] = ''
                                    parseInfo['classDescription'] = ''
                                    parseInfo['addConstructor'] = ''
                                    parseInfo['linkedClass'] = ''
                                    parseInfo['latexText'] = ''
                                    parseInfo['writePybindIncludes'] = 'False'
                                else:
                                    print("ERROR: did not expect 'writeFile' keyword in line",linecnt)    
                                    continueOperation = False
                                
                        else:
                            print("ERROR: definition mismatch in line",linecnt)    
                            continueOperation = False
                    else: #ERROR
                        print("ERROR: unknown format of line",linecnt)    
            linecnt += 1;
    
    if (continueOperation == False):
        print('\n\nERROR: Parsing terminated unexpectedly in line',linecnt,'\n\n')

    print('total number of lines generated =',totalNumberOfLines)
    
    fileLatex=open(latexFile,'w')  #clear file by one write access
    fileLatex.write('% definition of structures\n')
    fileLatex.write(globalLatexStr)
    fileLatex.close()

finally:    
    file.close()



