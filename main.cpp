#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

long RNN = -1; // متغير عالمي لتخزين الـ offset للسجل المحذوف الأخير

// تعريف عقدة AVAIL LIST (Linked List)
struct AvailNode {
    size_t size;    // حجم السجل
    long offset;    // الموقع (byte offset)
    AvailNode* next; // مؤشر للعقدة التالية
};

// رأس القائمة (المؤشر إلى أول عقدة)
AvailNode* availListHead = nullptr;

// دالة لعرض محتويات الـ AVAIL LIST
void displayAvailListWithContent(fstream &file) {
    if (availListHead == nullptr) {
        cout << "AVAIL LIST is empty.\n";
        return;
    }

    cout << "AVAIL LIST contents:\n";
    cout << "Size\tOffset\tContent\n";
    cout << "-----------------------------------------\n";

    AvailNode* current = availListHead;
    while (current != nullptr) {
        // الانتقال إلى الموقع المناسب في الملف
        file.seekg(current->offset, ios::beg);

        // قراءة السجل
        string content(current->size, '\0');
        file.read(&content[0], current->size);

        // عرض التفاصيل
        cout << current->size << "\t" << current->offset << "\t" << content << "\n";

        // الانتقال إلى العقدة التالية
        current = current->next;
    }
}
void addRecord(fstream &file, const string &record) {
    int recordSize = record.size(); // حجم السجل الجديد

    if (RNN == -1) {
        // RNN = -1 يعني لا توجد سجلات محذوفة
        // الانتقال إلى نهاية الملف
        file.seekp(0, ios::end);
        int offset = file.tellp(); // تحديد موقع السجل الجديد

        // كتابة السجل إلى الملف
        file.write(record.c_str(), recordSize);

        // إضافة السجل إلى AVAIL LIST
        AvailNode* newNode = new AvailNode{static_cast<size_t>(recordSize), offset, availListHead};
        availListHead = newNode; // إضافة العقدة في بداية القائمة

        cout << "Record added at the end of the file and registered in AVAIL LIST.\n";
    } else {
        // RNN يشير إلى سجل محذوف
        file.seekg(RNN, ios::beg);

        // قراءة العلامة "*" والأوفست للسجل المحذوف
        string header;
        file >> header;

        // التحقق إذا كانت العلامة موجودة
        if (header[0] != '*') {
            cout << "Invalid deleted record format at offset " << RNN << ".\n";
            return;
        }

        // استخراج حجم السجل المحذوف
        int deletedSize = file.tellg() ; // حساب الحجم المتاح

        if (deletedSize >= recordSize) {
            // إذا كان السجل المحذوف مناسبًا للسجل الجديد
            file.seekp(RNN, ios::beg);
            file.write(record.c_str(), recordSize);

            // تحديث RNN ليشير إلى السجل المحذوف التالي
            string nextDeletedOffsetStr = header.substr(1); // استخراج الأوفست التالي
            RNN = (nextDeletedOffsetStr == "-1") ? -1 : stoi(nextDeletedOffsetStr);

            cout << "Record added in deleted space at offset " << RNN << " and RNN updated.\n";
        } else {
            // إذا كان الحجم غير كافٍ، أضف السجل في نهاية الملف
            file.seekp(0, ios::end);
            int offset = file.tellp(); // تحديد موقع السجل الجديد

            file.write(record.c_str(), recordSize);

            // إضافة السجل الجديد إلى AVAIL LIST
            AvailNode* newNode = new AvailNode{static_cast<size_t>(recordSize), offset, availListHead};
            availListHead = newNode; // إضافة العقدة في بداية القائمة

            cout << "Record size not suitable for deleted space. Added at the end of the file.\n";
        }
    }
}


void deleteRecord(fstream &file, int offset, int size) {
    // التحقق من أن الحجم والموقع صالحان
    if (offset < 0 || size <= 0) {
        cout << "Invalid offset or size for deletion.\n";
        return;
    }

    // قراءة الجزء الأول من السجل للتحقق من العلامة '*'
    file.seekg(offset, ios::beg); // الانتقال إلى الموقع المحدد
    char firstChar;
    file.read(&firstChar, 1); // قراءة أول حرف

    if (firstChar == '*') {
        cout << "Record already deleted at offset: " << offset << "\n";
        return; // السجل محذوف مسبقًا، إيقاف العملية
    }

    // تحديد الـ offset للسجل المحذوف سابقًا إذا كان موجودًا
    string previousDeletedRecord = (RNN != -1) ? to_string(RNN) : to_string(-1);

    // بناء النص المطلوب الذي يحتوي فقط على * والـ offset السابق
    string deleteMarker = "*" + previousDeletedRecord; // بناء النص المطلوب
    cout << "Deleting record. Marker: " << deleteMarker << "\n";

    // تحديد الحجم الصحيح للنص المحذوف
    int deleteSize = deleteMarker.size(); // الحجم الصحيح للعلامة

    // إنشاء سلسلة تحتوي على الفراغات لتعبئة المساحة المتبقية
    string padding(size - deleteSize, ' '); // الفراغات المطلوبة لتعبئة المساحة

    // دمج النص المحذوف مع الفراغات
    string finalContent = deleteMarker + padding;

    // الانتقال إلى الموقع المناسب وكتابة المحتوى الجديد
    file.seekp(offset, ios::beg);
    file.write(finalContent.c_str(), size); // كتابة المحتوى مع الفراغات

    // تحديث RNN ليحمل قيمة الـ offset للسجل المحذوف
    RNN = offset;

    cout << "Record deleted and updated with 'Offset of previous deleted record': " << offset << "\n";
}


int main() {
    fstream file("data.dat", ios::in | ios::out | ios::binary | ios::trunc);

    // إضافة سجلات
    addRecord(file, "Eslam");
    addRecord(file, "Mazen");
    addRecord(file, "Ahmed");
    addRecord(file, "Cramer");
    addRecord(file, "Omar");
    addRecord(file, "Ali");
    displayAvailListWithContent(file);
    // حذف سجل
    //deleteRecord(file, 21, 4);
    //displayAvailListWithContent(file);

    deleteRecord(file, 5, 5);
    deleteRecord(file, 10, 5);
    deleteRecord(file, 15, 6);// حذف أول سجل
    displayAvailListWithContent(file);
    addRecord(file, "Ahmed");
    displayAvailListWithContent(file);
    addRecord(file, "Ahdmed");
    displayAvailListWithContent(file);
    // إضافة سجل جديد (سيستخدم المساحة المحذوفة إذا كانت كافية)
    // addRecord(file, "Doctor3: Alice Brown");

    file.close();
    return 0;
}
