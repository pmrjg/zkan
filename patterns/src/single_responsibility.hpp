//
// Created by pmrj on 01-01-2025.
//

using namespace std;
using namespace boost;

struct Journal {
    string title;
    vector<string> entries;

    explicit Journal(const string &title) : title(title) {}

    void add_entry(const string& entry) {
        static int count = 1;
        entries.push_back(lexical_cast<string>(count++) + ":" + entry);
    }


};

struct PersistenceManager {
    static void save(const Journal& j, const string &filename) {
        ofstream out(filename);
        for (const auto &entry : j.entries) {
            out << entry << endl;
        }
    }
};