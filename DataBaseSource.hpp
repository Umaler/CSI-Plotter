#pragma once

#include <string>

#include "DataBaseSource.hpp"

class DBSource : public DataSource {
public:

    virtual operator bool();

    virtual DataSource& operator>>(std::pair<double, double>&);
    virtual std::shared_ptr< std::vector<double> > getDataSet();

    struct amplitude {
        int id;
        int id_packet;
        int id_measurement;
        int num_sub;
        double ffa;
        double fsa;
        double fta;
        double sfa;
        double ssa;
        double sta;
        double tfa;
        double tsa;
        double tta;
    };

    struct clear_phase {
        int id;
        int id_packet;
        int id_measurement;
        int num_sub;
        double ffph;
        double fsph;
        double ftph;
        double sfpj;
        double ssph;
        double stph;
        double tfph;
        double tsph;
        double ttph;
    };

    struct measurement {
        int id;
        int id_packet;
        int num_sub;
        int ffr;
        int ffi;
        int fsr;
        int fsi;
        int ftr;
        int fti;
        int sfr;
        int sfi;
        int ssr;
        int ssi;
        int str;
        int sti;
        int tfr;
        int tfi;
        int tsr;
        int tsi;
        int ttr;
        int tti;
    };

    struct packet {
        int id;
        int marker;
        std::string description;
    };

    struct phase {
        int id;
        int id_packet;
        int id_measurement;
        int num_sub;
        double ffph;
        double fsph;
        double ftph;
        double sfph;
        double ssph;
        double stph;
        double tfph;
        double tsph;
        double ttph;
    };

private:


};
