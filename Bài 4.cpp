/****************/
// Mã sinh viên: 202418900
// Họ tên: Nguyễn Đăng Hiếu
/****************/

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#endif

// Lớp cơ sở Employee
class Employee {
protected:
    std::string employeeId;
    std::string fullName;
    std::string department;
    double monthlyBonus;

public:
    Employee(const std::string& id, const std::string& name) 
        : Employee(id, name, "Unassigned") {}

    Employee(const std::string& id, const std::string& name, const std::string& dept) {
        if (id.empty()) {
            throw std::invalid_argument("Ma nhan su khong duoc rong");
        }
        if (name.empty()) {
            throw std::invalid_argument("Ho ten khong duoc rong");
        }
        if (dept.empty()) {
            throw std::invalid_argument("Phong ban khong duoc rong");
        }
        
        this->employeeId = id;
        this->fullName = name;
        this->department = dept;
        this->monthlyBonus = 0.0;
    }

    std::string getId() const { return employeeId; }
    std::string getDepartment() const { return department; }

    void addBonus(double amount) {
        if (amount <= 0) {
            throw std::invalid_argument("Tien thuong phai lon hon 0");
        }
        monthlyBonus += amount;
    }

    void addBonus(double amount, const std::string& reason) {
        if (reason.empty()) {
            throw std::invalid_argument("Ly do thuong khong duoc rong");
        }
        addBonus(amount);
    }

    void addBonus(double rate, double referenceAmount, const std::string& reason) {
        if (rate <= 0 || rate > 0.5) {
            throw std::invalid_argument("Ty le thuong phai thuoc (0, 0.5]");
        }
        if (referenceAmount <= 0) {
            throw std::invalid_argument("Gia tri tham chieu phai lon hon 0");
        }
        if (reason.empty()) {
            throw std::invalid_argument("Ly do thuong khong duoc rong");
        }
        monthlyBonus += (rate * referenceAmount);
    }

    virtual double calculateGrossPay() const = 0;
    virtual std::string getEmployeeType() const = 0;
    
    virtual void displayPayrollInfo() const {
        std::cout << std::left << std::setw(10) << getEmployeeType() << " | "
                  << std::setw(6) << employeeId << " | "
                  << std::setw(16) << fullName << " | "
                  << std::setw(12) << department << " | "
                  << std::right << std::setw(10) << std::fixed << std::setprecision(0) 
                  << calculateGrossPay() << " VND\n";
    }

    virtual ~Employee() {
        std::cout << "[Huy] Employee " << employeeId << " - " << fullName << "\n";
    }
};

// Lớp SalariedEmployee
class SalariedEmployee : public Employee {
private:
    double monthlySalary;
    double responsibilityAllowance;

public:
    SalariedEmployee(const std::string& id, const std::string& name) 
        : SalariedEmployee(id, name, "Unassigned", 0, 0) {}

    SalariedEmployee(const std::string& id, const std::string& name, const std::string& dept, 
                     double salary, double allowance)
        : Employee(id, name, dept) {
        if (salary < 0 || allowance < 0) {
            throw std::invalid_argument("Luong va phu cap khong duoc am");
        }
        this->monthlySalary = salary;
        this->responsibilityAllowance = allowance;
    }

    double calculateGrossPay() const override {
        return monthlySalary + responsibilityAllowance + monthlyBonus;
    }

    std::string getEmployeeType() const override { 
        return "Salaried"; 
    }

    ~SalariedEmployee() override {
        std::cout << "[Huy] SalariedEmployee " << employeeId << " - " << fullName << "\n";
    }
};

// Lớp HourlyEmployee
class HourlyEmployee : public Employee {
private:
    double hourlyRate;
    double workedHours;

public:
    HourlyEmployee(const std::string& id, const std::string& name) 
        : HourlyEmployee(id, name, "Unassigned", 0, 0) {}

    HourlyEmployee(const std::string& id, const std::string& name, const std::string& dept, 
                   double rate, double hours)
        : Employee(id, name, dept) {
        if (rate < 0) {
            throw std::invalid_argument("Don gia gio khong duoc am");
        }
        if (hours < 0 || hours > 250) {
            throw std::invalid_argument("So gio lam phai tu 0 den 250");
        }
        this->hourlyRate = rate;
        this->workedHours = hours;
    }

    double calculateGrossPay() const override {
        double basePay = 0;
        if (workedHours <= 160) {
            basePay = workedHours * hourlyRate;
        } else {
            basePay = 160 * hourlyRate + (workedHours - 160) * hourlyRate * 1.5;
        }
        return basePay + monthlyBonus;
    }

    std::string getEmployeeType() const override { 
        return "Hourly"; 
    }

    ~HourlyEmployee() override {
        std::cout << "[Huy] HourlyEmployee " << employeeId << " - " << fullName << "\n";
    }
};

// Lớp SalesEmployee
class SalesEmployee : public Employee {
private:
    double baseSalary;
    double salesRevenue;
    double commissionRate;

public:
    SalesEmployee(const std::string& id, const std::string& name)
        : SalesEmployee(id, name, "Unassigned", 0, 0, 0) {}

    SalesEmployee(const std::string& id, const std::string& name, const std::string& dept, 
                  double salary, double revenue, double commission)
        : Employee(id, name, dept) {
        if (salary < 0 || revenue < 0) {
            throw std::invalid_argument("Luong CB va doanh so khong duoc am");
        }
        if (commission < 0 || commission > 0.3) {
            throw std::invalid_argument("Ty le hoa hong phai tu 0 den 0.3");
        }
        this->baseSalary = salary;
        this->salesRevenue = revenue;
        this->commissionRate = commission;
    }

    double calculateGrossPay() const override {
        return baseSalary + (salesRevenue * commissionRate) + monthlyBonus;
    }

    std::string getEmployeeType() const override { 
        return "Sales"; 
    }

    ~SalesEmployee() override {
        std::cout << "[Huy] SalesEmployee " << employeeId << " - " << fullName << "\n";
    }
};

// Lớp Payroll
class Payroll {
private:
    std::string period;
    std::vector<Employee*> employees;

public:
    Payroll(const std::string& p) : period(p) {
        if (p.empty()) {
            throw std::invalid_argument("Ky luong khong duoc rong");
        }
    }

    Employee* findEmployee(const std::string& id) const {
        for (size_t i = 0; i < employees.size(); ++i) {
            if (employees[i]->getId() == id) {
                return employees[i];
            }
        }
        return nullptr;
    }

    bool addEmployee(Employee* employee) {
        if (employee == nullptr) return false;
        
        if (findEmployee(employee->getId()) != nullptr) {
            std::cout << "[Tu choi] Nhan su " << employee->getId() << " da co trong bang luong\n";
            return false;
        }
        employees.push_back(employee);
        std::cout << "[OK] Da them " << employee->getId() << " vao ky luong " << period << "\n";
        return true;
    }

    double calculateTotalPayroll() const {
        double total = 0;
        for (size_t i = 0; i < employees.size(); ++i) {
            total += employees[i]->calculateGrossPay();
        }
        return total;
    }

    double calculatePayrollByDepartment(const std::string& dept) const {
        double total = 0;
        for (size_t i = 0; i < employees.size(); ++i) {
            if (employees[i]->getDepartment() == dept) {
                total += employees[i]->calculateGrossPay();
            }
        }
        return total;
    }

    Employee* findHighestPaidEmployee() const {
        if (employees.empty()) return nullptr;
        Employee* highest = employees[0];
        for (size_t i = 1; i < employees.size(); ++i) {
            if (employees[i]->calculateGrossPay() > highest->calculateGrossPay()) {
                highest = employees[i];
            }
        }
        return highest;
    }

    void displayPayroll() const {
        std::cout << "Ky luong: " << period << " | Tong nhan su: " << employees.size() << "\n";
        if (employees.empty()) {
            std::cout << "Danh sach trong\n";
            return;
        }
        for (size_t i = 0; i < employees.size(); ++i) {
            employees[i]->displayPayrollInfo();
        }
        std::cout << "Tong chi phi luong ki nay: " << std::fixed << std::setprecision(0) 
                  << calculateTotalPayroll() << " VND\n";
    }
};

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif

    std::cout << "[1] Nhap du lieu 4 nhan vien theo de bai\n";
    SalariedEmployee e1("E001", "Nguyen Minh An", "Dao tao", 15000000, 2000000);
    e1.addBonus(1000000); 

    HourlyEmployee e2("E002", "Tran Thu Binh", "Ho tro", 100000, 150);
    e2.addBonus(500000, "Thuong hoan thanh tot");

    HourlyEmployee e3("E003", "Le Hoang Chi", "Ho tro", 100000, 170);

    SalesEmployee e4("E004", "Pham Quoc Dung", "Kinh doanh", 8000000, 200000000, 0.05);
    e4.addBonus(0.02, 50000000, "Thuong vuot KPI");

    std::cout << "\n[2] Dua nhan vien vao Payroll\n";
    Payroll pr("2026-09");
    pr.addEmployee(&e1);
    pr.addEmployee(&e2);
    pr.addEmployee(&e3);
    pr.addEmployee(&e4);

    std::cout << "\n[3] Hien thi bang luong\n";
    pr.displayPayroll();

    std::cout << "\n[4] Tinh toan thong ke\n";
    std::cout << "Chi phi phong 'Ho tro': " << pr.calculatePayrollByDepartment("Ho tro") << " VND\n";
    
    Employee* highest = pr.findHighestPaidEmployee();
    if (highest != nullptr) {
        std::cout << "Nguoi thu nhap cao nhat: " << highest->getId() << " - " 
                  << highest->calculateGrossPay() << " VND\n";
    }

    std::cout << "\nCac truong hop bien\n";
    std::cout << "Them trung ma: ";
    pr.addEmployee(&e1);

    try {
        SalariedEmployee err("", "Loi Ma", "HR", 1000, 100);
    } catch (const std::exception& e) {
        std::cout << "[Loi bat duoc] " << e.what() << "\n";
    }

    try {
        HourlyEmployee err("E99", "Loi Gio", "IT", 100, 300);
    } catch (const std::exception& e) {
        std::cout << "[Loi bat duoc] " << e.what() << "\n";
    }

    try {
        SalesEmployee err("E98", "Loi Ty le", "Sale", 1000, 100, 0.5);
    } catch (const std::exception& e) {
        std::cout << "[Loi bat duoc] " << e.what() << "\n";
    }

    try {
        e1.addBonus(-500);
    } catch (const std::exception& e) {
        std::cout << "[Loi bat duoc] " << e.what() << "\n";
    }

    try {
        e1.addBonus(100, "");
    } catch (const std::exception& e) {
        std::cout << "[Loi bat duoc] " << e.what() << "\n";
    }

    try {
        e1.addBonus(0.8, 1000, "Loi ty le lon hon 0.5");
    } catch (const std::exception& e) {
        std::cout << "[Loi bat duoc] " << e.what() << "\n";
    }

    try {
        Payroll errPR("");
    } catch (const std::exception& e) {
        std::cout << "[Loi bat duoc] " << e.what() << "\n";
    }

    std::cout << "Hien thi Payroll rong:\n";
    Payroll emptyPR("2026-10");
    emptyPR.displayPayroll();
    
    std::cout << "Tim nguoi cao nhat trong Payroll rong: ";
    if (emptyPR.findHighestPaidEmployee() == nullptr) {
        std::cout << "Tra ve Null\n";
    }

    std::cout << "\nKet thuc main, cac doi tuong se tu huy\n";
    return 0;
} 