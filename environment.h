#ifndef ENV
#define ENV

#include <unordered_map>
#include <list>
#include <vector>
#include <string>
#include <iostream>

using namespace std;
class Environment
{
private:
    vector<unordered_map<string, int>> int_levels;
    vector<unordered_map<string, float>> float_levels;
    vector<unordered_map<string, string>> string_levels;
    vector<unordered_map<string, string>> type_levels;

    int search_rib(string var, string type)
    {
        if (type == "Int")
        {
            int idx = int_levels.size() - 1;
            while (idx >= 0)
            {
                if (int_levels[idx].find(var) != int_levels[idx].end())
                {
                    return idx;
                }
                idx--;
            }
        }
        else if (type == "Float")
        {
            int idx = float_levels.size() - 1;
            while (idx >= 0)
            {
                if (float_levels[idx].find(var) != float_levels[idx].end())
                {
                    return idx;
                }
                idx--;
            }
        }
        else if (type == "String")
        {
            int idx = string_levels.size() - 1;
            while (idx >= 0)
            {
                if (string_levels[idx].find(var) != string_levels[idx].end())
                {
                    return idx;
                }
                idx--;
            }
        }
        return -1;
    }

public:
    Environment() {}

    void add_var(string var, int value, string type)
    {
        if (type == "Int")
        {
            int_levels.back()[var] = value;
            type_levels.back()[var] = "Int";
        }
        else if (type == "Float")
        {
            float_levels.back()[var] = static_cast<float>(value);
            type_levels.back()[var] = "Float";
        }
    }

    void add_var(string var, float value, string type)
    {
        if (type == "Float")
        {
            float_levels.back()[var] = value;
            type_levels.back()[var] = "Float";
        }
    }

    void add_var(string var, string value, string type)
    {
        if (type == "String")
        {
            string_levels.back()[var] = value;
            type_levels.back()[var] = "String";
        }
    }

    void add_var(string var, string type)
    {
        if (type == "Int")
        {
            int_levels.back()[var] = 0;
            type_levels.back()[var] = "Int";
        }
        else if (type == "Float")
        {
            float_levels.back()[var] = 0.0f;
            type_levels.back()[var] = "Float";
        }
    }

    void update(string var, int value)
    {
        int idx = search_rib(var, "Int");
        if (idx != -1)
        {
            int_levels[idx][var] = value;
        }
        else
        {
            int idf = search_rib(var, "Float");
            if (idf != -1)
            {
                float_levels[idf][var] = static_cast<float>(value);
            }
        }
    }

    void update(string var, float value)
    {
        int idx = search_rib(var, "Float");
        if (idx != -1)
        {
            float_levels[idx][var] = value;
        }
    }

    void update(string var, string value)
    {
        int idx = search_rib(var, "String");
        if (idx != -1)
        {
            string_levels[idx][var] = value;
        }
    }

    int lookup(string var)
    {
        int idx = search_rib(var, "Int");
        if (idx != -1)
            return int_levels[idx][var];

        int idxf = search_rib(var, "Float");
        if (idxf != -1)
            return static_cast<int>(float_levels[idxf][var]);
        return 0;
    }

    float lookup_float(string var)
    {
        int idx = search_rib(var, "Float");
        if (idx != -1)
            return float_levels[idx][var];

        int idxi = search_rib(var, "Int");
        if (idxi != -1)
            return static_cast<float>(int_levels[idxi][var]);
        return 0.0f;
    }

    string lookup_string(string var)
    {
        int idx = search_rib(var, "String");
        if (idx != -1)
            return string_levels[idx][var];
        return "";
    }

    string lookup_type(string var)
    {
        int n = type_levels.size() - 1;
        while (n >= 0)
        {
            if (type_levels[n].find(var) != type_levels[n].end())
            {
                return type_levels[n][var];
            }
            n--;
        }
        return "";
    }

    bool check(string var)
    {
        int n = type_levels.size() - 1;
        while (n >= 0)
        {
            if (type_levels[n].find(var) != type_levels[n].end())
            {
                return true;
            }
            n--;
        }
        return false;
    }

    void add_level()
    {
        int_levels.push_back(unordered_map<string, int>());
        float_levels.push_back(unordered_map<string, float>());
        string_levels.push_back(unordered_map<string, string>());
        type_levels.push_back(unordered_map<string, string>());
    }
    void remove_level()
    {
        int_levels.pop_back();
        float_levels.pop_back();
        string_levels.pop_back();
        type_levels.pop_back();
    }

    bool typecheck(const string &var, const string &expected_type)
    {
        string actual_type = lookup_type(var);
        if (actual_type != expected_type)
        {
            cerr << "Error de tipo: se esperaba " << expected_type
                 << " pero se encontró " << actual_type
                 << " para la variable " << var << endl;
            return false;
        }
        return true;
    }
};

#endif
