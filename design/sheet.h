#pragma once
#include <iostream>
#include "cell.h"
#include "common.h"

#include <functional>
#include <unordered_map>
#include <vector>

class Sheet : public SheetInterface {
public:
    ~Sheet();
   
    void SetCell(Position pos, std::string text, Sheet* sheet = this) override{   	
}

    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;

    void ClearCell(Position pos) override{
	проверяем наличие ячейки с такой позиции в ключах cell_dependencies.
	если есть,то пробегаем по значениям всех ячеек для заданного ключа и меняем is_valid_ на false с помощью метода Validate.
	удаляем соответствующий ключ из cell_dependencies	
	очищаем ячейку и удаляем соответствующую позицию из cells_posit
	ячейка везде в формулах теперь будет интерпретироваться как 0.
};

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;  

    void IsCyclicDependenced(const Sheet& sheet,const Position& curent_cell, vector<Position> UpperNodeCells = {}){
	UpperNodeCells.push_back(curent_cell);
	vector<Position> ReferencedCells = (sheet.GetCell(curent_cell))->GetReferencedCells();
	if (ReferencedCells.size()==0){};
	else{
		for(const auto& cell:ReferencedCells){
			if (count(UpperNodeCells.begin(),UpperNodeCells.end(),cell)) throw CircularDependencyException("");
			else IsCyclicDependenced(sheet,cell,UpperNodeCells);

	     }
	}

    }  
   
   
private:
    // Можете дополнить ваш класс нужными полями и методами   
    std::vector<std::vector<std::unique_ptr<Cell>>> sheet;
    std::set<Position> cells_posit;   
    std::unordered_map<Position,vector<Position>> cell_dependencies //славрь, отображающий при расчете каких ячеек учавствует	ключ ячейка	
};