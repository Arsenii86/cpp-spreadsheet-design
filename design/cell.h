#pragma once

#include "common.h"
#include "formula.h"
#include "sheet.h"

class Impl{
    public:
        virtual CellInterface::Value GetValue()const=0;
        virtual std::string GetText()const=0;
        virtual void Clear()=0;
        virtual ~Impl() = default;
};

class EmptyImpl: public Impl{
        std::string empty_value_={};
    public:
        EmptyImpl(){};
        CellInterface::Value GetValue() const{
            return empty_value_;
        }
        std::string GetText() const{
            return empty_value_;
        }
    
        void Clear(){ 
            empty_value_.clear();
        }
        ~EmptyImpl()=default;
    
 };

class TextImpl: public Impl{
        std::string value_;
    public:
        TextImpl(const std::string& text):value_(text){};
        CellInterface::Value GetValue() const{
            if (value_[0]==ESCAPE_SIGN){
               return std::string(value_.begin()+1,value_.end()); 
            }
            return value_;
        }
        std::string GetText() const{
            return value_;
        }
    
        void Clear(){
            value_.clear();
        };
        ~TextImpl()=default;
    
 };

class FormulaImpl: public Impl{
        std::unique_ptr<FormulaInterface> formula_uniq_ptr; 	
   
    public:
        FormulaImpl(std::string formula_text, Sheet* sheet_ptr ):
            formula_uniq_ptr(std::move(ParseFormula(formula_text))){      //Может выбросить исключение FormulaException {"Uncorrect formula"}
		SetSheetPtr(Sheet*  sheet_ptr);//передал указатель на лист в formulu 
	};
    
        CellInterface::Value GetValue() const{            
            const auto value = formula_uniq_ptr->Evaluate();
            if (std::holds_alternative<double>(value)) return std::get<double>(value);
            else return std::get<FormulaError>(value);
        }
       
	std::vector<Position> GetCellFromFormula(){
		if (!formula_uniq_ptr) return std::vector<Position>{}; 
		else return  formula_uniq_ptr ->GetCellFromFormula();

};

        std::string GetText() const{
            return '='+formula_uniq_ptr->GetExpression();
        }
    
        void Clear(){
            formula_uniq_ptr.reset();
        };
        ~FormulaImpl()=default;
    
    
};


class Cell : public CellInterface {
public:
    //Cell(Sheet& sheet_ref):sheet_ref_(Sheet& sheet_ref);
    Cell(Sheet* sheet_ptr):sheet_ptr_(sheet_ptr){};
    ~Cell()=default;

    void Cell::Set(std::string text) {
    if(text.size() == 0){
        //создаем указатель на пустую текстовую ячейку
        Clear();
        std::unique_ptr<EmptyImpl> empty_impl = std::make_unique<EmptyImpl>();
        impl_=std::move(empty_impl);
        //impl_=empty_impl;
    }
    else if (text.size() > 1 && text[0] == FORMULA_SIGN){        
        //воспринимаем как фомулу и создаем указатель на формульную ячейку
        std::string formula(text.begin()+1,text.end());
        std::unique_ptr<FormulaImpl> formula_impl = std::make_unique<FormulaImpl>(formula);		
	formula_impl -> SetSheetPtr(sheet_ptr); /////////передаем вглубь указатели на лист 					
        impl_= std::move(formula_impl);
        
    }    
    else if(text.size() == 1 && text[0] == FORMULA_SIGN){
        //воспринимаем как текст и создаем указатель на TextImpl
        std::unique_ptr<TextImpl> text_impl = std::make_unique<TextImpl>(std::move(text));
        impl_=std::move(text_impl);
       
    }    
    else{
         //воспринимаем как текст и создаем указатель на текстовую ячейку        
        Clear();       
        std::unique_ptr<TextImpl> text_impl = std::make_unique<TextImpl>(std::move(text));
        impl_=std::move(text_impl);
        //impl_=text_impl;
    }
}
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;

    std::vector<Position> GetReferencedCells(){
	if (!impl_) return std::vector<Position>{};
	else return  impl_-> GetCellFromFormula();
};

    void Validate(bool valid);

private:
 std::unique_ptr<Impl> impl_; 
 Sheet* sheet_ptr_;
 bool is_valid_;валидность вычисленного значения
 Value temp_;кеш вычисленного значения
 

};



