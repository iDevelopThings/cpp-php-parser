//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_NEWAST_ASTNODE_H
#define INTERPRETER_NEWAST_ASTNODE_H

#include <vector>
#include <functional>
#include <utility>
#include <fstream>
#include <iostream>
#include <sstream>


namespace AST {

    class Position {

    private:
        // The absolute position of the token in the file
        int pos;
        // The absolute position of the beginning of the line
        int bol;

    public:

        Position(int pos, int bol) : pos(pos), bol(bol) {}

        friend std::ostream &operator<<(std::ostream &os, const AST::Position &obj) {
            os << "Position: " << obj.pos << " " << obj.bol << "\n";
            return os;
        };

        int getColumn() const { return pos - bol; }

        int getLine() const { return bol; }

        int getAbsolute() const { return pos; }
    };


    class ASTNode {
    public:
        Position* position {};
        std::vector<ASTNode*> nodes;

        ASTNode() = default;


        explicit ASTNode(Position* position) : position(position) {
            this->nodes = std::vector<ASTNode*>();
        }

        virtual void print_info(std::ostream &os, int indent) const;

        friend std::ostream &operator<<(std::ostream &os, const ASTNode &obj) {
            obj.print_info(os, 2);
            return os;
        }

        void addNode(ASTNode* node);

        void addNodes(std::vector<ASTNode*> pNodes);

        std::vector<ASTNode*> getNodes() { return nodes; }

        virtual void visit(const std::function<void(ASTNode*)> &fn);


        virtual void to_json(std::ostream &os, int indent);

    };

}


#endif //INTERPRETER_NEWAST_ASTNODE_H

