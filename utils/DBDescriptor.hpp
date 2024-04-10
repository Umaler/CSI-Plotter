#pragma once

#include <vector>
#include <string>
#include <initializer_list>

#include <SQLiteCpp/Database.h>

class DBDescriptor {
public:
    struct Table {
        typedef std::vector<std::string> Fields;

        std::string name;
        Fields fields;

        Fields::iterator begin();
        Fields::const_iterator begin() const;
        Fields::iterator end();
        Fields::const_iterator end() const;
    };
    typedef std::vector<Table> Tables;

    DBDescriptor(std::initializer_list<Table> il);

    Tables::iterator begin();
    Tables::const_iterator begin() const;
    Tables::iterator end();
    Tables::const_iterator end() const;

    bool checkCompliance(SQLite::Database& db) const;

private:
    Tables tables;

};

inline const DBDescriptor dbDescriptor
{
    {
        "amplitude",
        {
            "ffa",
            "fsa",
            "fta",
            "sfa",
            "ssa",
            "sta",
            "tfa",
            "tsa",
            "tta"
        }
    },
    {
        "clear_phase",
        {
            "ffph",
            "fsph",
            "ftph",
            "sfph",
            "ssph",
            "stph",
            "tfph",
            "tsph",
            "ttph"
        }
    },
    /*{
        "measurement",
        {
            "ffr",
            "ffi",
            "fsr",
            "fsi",
            "ftr",
            "fti",
            "sfr",
            "sfi",
            "ssr",
            "ssi",
            "str",
            "sti",
            "tfr",
            "tfi",
            "tsr",
            "tsi",
            "ttr",
            "tti"
        }
    },*/
    {
        "phase",
        {
            "ffph",
            "fsph",
            "ftph",
            "sfph",
            "ssph",
            "stph",
            "tfph",
            "tsph",
            "ttph"
        }
    }
};
