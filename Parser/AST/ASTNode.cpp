#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

//
// Created by Sam on 10/06/2023.
//

#include <iostream>
#include "ASTNode.h"

void AST::ASTNode::addNode(AST::ASTNode* node) {
    this->nodes.push_back(node);
}

void AST::ASTNode::visit(const std::function<void(ASTNode*)> &fn) {
    fn(this);
    for (auto node: nodes) {
        node->visit(fn);
    }
}

void AST::ASTNode::addNodes(std::vector<ASTNode*> pNodes) {
    this->nodes.insert(this->nodes.end(), pNodes.begin(), pNodes.end());
}


#pragma clang diagnostic pop

void AST::ASTNode::print_info(std::ostream &os, int indent) const {
    os << "ASTNode(" << typeid(this).name() << "): " << position << std::endl;
    if (!nodes.empty()) {
        os << "Nodes: " << std::endl;
        for (auto node: nodes) {
            os << *node << std::endl;
        }
    } else {
        os << "No nodes" << std::endl;
    }
}

void AST::ASTNode::to_json(std::ostream &os, int indent) {
    std::string ident(indent, ' ');
    os << ident << "{\n";
    os << ident << "  \"type\": \"" << typeid(this).name() << "\",\n";
    os << ident << "  \"position\": " << position << ",\n";
    if (!nodes.empty()) {
        os << ident << "  \"nodes\": [\n";
        for (auto node: nodes) {
            node->to_json(os, indent + 4);
            os << ",\n";
        }
        os << ident << "  ]\n";
    } else {
        os << ident << "  \"nodes\": []\n";
    }
    os << ident << "}";
}
