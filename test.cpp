#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdexcept>

class Cell {
private:
    std::string m_value;

public:
    bool isNumericString(const std::string& value) {
        return std::all_of(value.begin(), value.end(), [](char c) {
            return std::isdigit(c);
        });
    }

    int toInt() {
        if (isNumericString(m_value)) {
            return std::stoi(m_value);
        } else {
            throw std::invalid_argument("Invalid number format");
        }
    }

    double toDouble() {
        if (isNumericString(m_value)) {
            return std::stod(m_value);
        } else {
            throw std::invalid_argument("Invalid number format");
        }
    }

    void setValue(int value) {
        m_value = std::to_string(value);
    }

    void setValue(double value) {
        m_value = std::to_string(value);
    }

    void setValue(const std::string& value) {
        m_value = value;
    }

    std::string getValue() const {
        return m_value;
    }
};

class Spreadsheet {
private:
    Cell** m_cells;
    int m_rows;
    int m_cols;

public:
    Spreadsheet(int rows, int cols) : m_rows(rows), m_cols(cols) {
        m_cells = new Cell*[m_rows];
        for (int i = 0; i < m_rows; ++i) {
            m_cells[i] = new Cell[m_cols];
        }
    }

    ~Spreadsheet() {
        for (int i = 0; i < m_rows; ++i) {
            delete[] m_cells[i];
        }
        delete[] m_cells;
    }

    void setCellAt(int row, int col, const std::string& value) {
        if (row >= 0 && row < m_rows && col >= 0 && col < m_cols) {
            m_cells[row][col].setValue(value);
        }
    }

    void setCellAt(int row, int col, Cell* cell) {
        if (row >= 0 && row < m_rows && col >= 0 && col < m_cols) {
            m_cells[row][col] = *cell;
        }
    }

    Cell& getCellAt(int row, int col) const {
        static Cell invalidCell;
        if (row >= 0 && row < m_rows && col >= 0 && col < m_cols) {
            return m_cells[row][col];
        } else {
            std::cout << "Invalid cell index: (" << row << ", " << col << ")\n";
            return invalidCell;
        }
    }

    void addRow(int numRows) {
        if (numRows <= 0) {
            return;
        }

        Cell** tempCells = new Cell*[m_rows + numRows];
        for (int i = 0; i < m_rows; ++i) {
            tempCells[i] = m_cells[i];
        }
        for (int i = m_rows; i < m_rows + numRows; ++i) {
            tempCells[i] = new Cell[m_cols];
        }
        delete[] m_cells;
        m_cells = tempCells;
        m_rows += numRows;
    }

    void addCol(int numCols) {
        if (numCols <= 0) {
            return;
        }

        for (int i = 0; i < m_rows; ++i) {
            Cell* tempCells = new Cell[m_cols + numCols];
            for (int j = 0;                j < m_cols; ++j) {
                tempCells[j] = m_cells[i][j];
            }
            delete[] m_cells[i];
            m_cells[i] = tempCells;
            tempCells = nullptr;
            for (int j = m_cols; j < m_cols + numCols; ++j) {
                m_cells[i][j] = Cell();
            }
        }
        m_cols += numCols;
    }

    int getNumRows() const {
        return m_rows;
    }

    int getNumCols() const {
        return m_cols;
    }
};

void testCell() {
    Cell cell;

    // Test isNumericString()
    if (!cell.isNumericString("123")) {
        std::cout << "isNumericString test failed: Expected true, got false\n";
    }
    if (cell.isNumericString("abc")) {
        std::cout << "isNumericString test failed: Expected false, got true\n";
    }
    if (cell.isNumericString("")) {
        std::cout << "isNumericString test failed: Expected false, got true\n";
    }

    // Test toInt()
    cell.setValue(42);
    if (cell.toInt() != 42) {
        std::cout << "toInt test failed: Expected 42, got " << cell.toInt() << "\n";
    }
    try {
        cell.setValue("abc");
        cell.toInt();
        std::cout << "toInt test failed: Expected exception, but no exception was thrown\n";
    } catch (const std::invalid_argument& e) {
        // Exception was thrown as expected
    }

    // Test toDouble()
    cell.setValue(3.14);
    if (cell.toDouble() != 3.14) {
        std::cout << "toDouble test failed: Expected 3.14, got " << cell.toDouble() << "\n";
    }
    try {
        cell.setValue("abc");
        cell.toDouble();
        std::cout << "toDouble test failed: Expected exception, but no exception was thrown\n";
    } catch (const std::invalid_argument& e) {
        // Exception was thrown as expected
    }
}

void testSpreadsheet() {
    Spreadsheet spreadsheet(3, 3);

    // Test setCellAt() and getCellAt()
    spreadsheet.setCellAt(0, 0, "10");
    if (spreadsheet.getCellAt(0, 0).toInt() != 10) {
        std::cout << "setCellAt/getCellAt test failed: Expected 10, got " << spreadsheet.getCellAt(0, 0).toInt() << "\n";
    }

    spreadsheet.setCellAt(1, 1, "3.14");
    if (spreadsheet.getCellAt(1, 1).toDouble() != 3.14) {
        std::cout << "setCellAt/getCellAt test failed: Expected 3.14, got " << spreadsheet.getCellAt(1, 1).toDouble() << "\n";
    }

    Cell cell;
    cell.setValue("Hello");
    spreadsheet.setCellAt(2, 2, &cell);
    if (spreadsheet.getCellAt(2, 2).getValue() != "Hello") {
        std::cout << "setCellAt/getCellAt test failed: Expected 'Hello', got " << spreadsheet.getCellAt(2, 2).getValue() << "\n";
    }

    // Test addRow()
    spreadsheet.addRow(2);
    if (spreadsheet.getNumRows() != 5) {
        std::cout << "addRow test failed: Expected 5 rows, got " << spreadsheet.getNumRows() << "\n";
    }
    if (spreadsheet.getCellAt(4, 2).getValue() != "") {
        std::cout << "addRow test failed: Expected empty value, got " << spreadsheet.getCellAt(4, 2).getValue() << "\n";
    }

    // Test addCol()
    spreadsheet.addCol(2);
    if (spreadsheet.getNumCols() != 5) {
        std::cout << "addCol test failed: Expected 5 columns, got " << spreadsheet.getNumCols() << "\n";
    }
    if (spreadsheet.getCellAt(2, 4).getValue() != "") {
        std::cout << "addCol test failed: Expected empty value, got " << spreadsheet.getCellAt(2, 4).getValue() << "\n";
    }
}

int main() {
    // Test the Cell class
    testCell();

    // Test the Spreadsheet class
    testSpreadsheet();

    return 0;
} 
