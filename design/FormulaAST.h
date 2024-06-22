#pragma once

#include "FormulaLexer.h"
#include "common.h"

#include <forward_list>
#include <functional>
#include <stdexcept>

namespace ASTImpl {
class Expr;
}

class ParsingError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class FormulaAST {
public:
	explicit FormulaAST(std::unique_ptr<ASTImpl::Expr> root_expr);
	FormulaAST(FormulaAST&&);
	FormulaAST& operator=(FormulaAST&&);
	~FormulaAST();
	double Execute(const SheetInterface& sheet) const{
		root_expr_->Evaluate(sheet );
};
	void Print(std::ostream& out) const;
	void PrintFormula(std::ostream& out) const; 
	std::vector<Position> GetCellFromFormulaAST(){
		return vector<Position>(cells_.begin(), cells_.end());
	};

private:
	std::unique_ptr<ASTImpl::Expr> root_expr_;
	std::forward_list<Position> cells_;


};

FormulaAST ParseFormulaAST(std::istream& in);
FormulaAST ParseFormulaAST(const std::string& in_str);