cmake_minimum_required(VERSION 3.25)
project(interpreter)

set(CMAKE_CXX_STANDARD 23)


add_executable(
        interpreter
        main.cpp
#        Lexer/Lexer.cpp Lexer/Lexer.h Lexer/Token.cpp Lexer/Token.h Lexer/TokenType.h
#        Parser/Parser.cpp Parser/Parser.h
#        AST/ASTNode.cpp AST/ASTNode.h
#        AST/Script.cpp AST/Script.h
#        AST/ClassDeclaration.cpp AST/ClassDeclaration.h
#        AST/FunctionDeclaration.cpp AST/FunctionDeclaration.h
#        AST/PropertyDeclaration.cpp AST/PropertyDeclaration.h
#        AST/NamespaceDeclaration.cpp AST/NamespaceDeclaration.h
#        AST/UseNamespaceDeclaration.h AST/UseNamespaceDeclaration.cpp
        FileSystem/Manager.cpp FileSystem/Manager.h
        timer.cpp timer.h
        Lexer/NewLexer.cpp Lexer/NewLexer.h
        Lexer/Token/TokenTypeMixin.cpp Lexer/Token/TokenTypeMixin.h
        Lexer/Token/TokenType.cpp Lexer/Token/TokenType.h
        Parser/NewParser.cpp Parser/NewParser.h
        Parser/AST/ASTNode.cpp Parser/AST/ASTNode.h
        Parser/AST/Script.cpp Parser/AST/Script.h
        Parser/AST/ClassDeclaration.cpp Parser/AST/ClassDeclaration.h
        Parser/AST/FunctionDeclaration.cpp Parser/AST/FunctionDeclaration.h
        Parser/AST/PropertyDeclaration.cpp Parser/AST/PropertyDeclaration.h
        Parser/AST/NamespaceDeclaration.cpp Parser/AST/NamespaceDeclaration.h
        Parser/AST/UseNamespaceDeclaration.h Parser/AST/UseNamespaceDeclaration.cpp
        Parser/AST/FullyQualifiedTypeName.cpp Parser/AST/FullyQualifiedTypeName.h Parser/AST/Attribute.cpp Parser/AST/Attribute.h Parser/AST/Parameter.cpp Parser/AST/Parameter.h Parser/AST/BaseObjectDeclaration.cpp Parser/AST/BaseObjectDeclaration.h Parser/AST/InterfaceDeclaration.cpp Parser/AST/InterfaceDeclaration.h Parser/AST/Statement.cpp Parser/AST/Statement.h Parser/AST/Expression.cpp Parser/AST/Expression.h)

