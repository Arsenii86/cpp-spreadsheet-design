#include "formula.h"

#include "FormulaAST.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <sstream>

using namespace std::literals;

std::ostream& operator<<(std::ostream& output, FormulaError fe) {
   const auto error_cat= fe.GetCategory();
     switch (error_cat){
	case(FormulaError::Category::Ref):{
		output <<"#REF!";
		break;
	}
	case(FormulaError::Category::Value):{
		output <<"#VALUE!";
		break;
	}
	case(FormulaError::Category::Arithmetic):{
		return output << "#ARITHM!";
		break;
	}
	default:
	{
		output << "#UNKNOW";
		break;
	}
    } 
}

namespace {
class Formula : public FormulaInterface {
public:
// Реализуйте следующие методы:
    explicit Formula(std::string expression) try
        :ast_(ParseFormulaAST(expression)) {
            
    } catch (const std::exception& exc) {
       throw FormulaException {"Uncorrect formula"};
    } 
    
    Value Evaluate(const SheetInterface& sheet = *sheet_ptr_ ) const override{
        try {  double temp = ast_.Execute(sheet);
           return temp;
        } catch (...) {
            return FormulaError ("Division by zero");
            //return FormulaError ("#ARITHM!");
        }
    } 
    
   

    std::string GetExpression() const override {
      std::stringstream out;
      ast_.PrintFormula(out);
      return out.str();          
}; 

    void SetSheetPtr(Sheet*  sheet_ptr){
	sheet_ptr_=sheet_ptr;
};

    std::vector<Position> GetCellFromFormula(){
	return ast_ ->GetCellFromFormulaAST();
};


private:
    FormulaAST ast_;
    Sheet*  sheet_ptr_;
};
}  // namespace

std::unique_ptr<FormulaInterface> ParseFormula(std::string expression) {
    return std::make_unique<Formula>(std::move(expression));
}