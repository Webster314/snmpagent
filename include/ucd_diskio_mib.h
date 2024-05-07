#ifndef _ucd_diskio_mib_h_
#define _ucd_diskio_mib_h_

#include <libagent.h>
#include <agent_pp/mib.h>
#include <agent_pp/snmp_textual_conventions.h>

#ifdef AGENTPP_NAMESPACE
namespace Agentpp {
    using namespace Snmp_pp;
#endif

#define oidUcdDiskIO        "1.3.6.1.4.1.2021.13.15"

#define offDiskIOReads      1
#define offDiskIOWrites     5

// Columns
#define oidDiskIODevice     "1.3.6.1.4.1.2021.13.15.1.1.2"
#define colDiskIODevice     "2"
#define oidDiskIOReads      "1.3.6.1.4.1.2021.13.15.1.1.5"
#define colDiskIOReads      "5"
#define oidDiskIOLA1        "1.3.6.1.4.1.2021.13.15.1.1.9"
#define colDiskIOLA1        "9"
#define oidDiskIOWrites     "1.3.6.1.4.1.2021.13.15.1.1.6"
#define colDiskIOWrites     "6"
// Tables
#define oidDiskIOTable      "1.3.6.1.4.1.2021.13.15.1"
#define oidDiskIOEntry      "1.3.6.1.4.1.2021.13.15.1.1"
#define nDiskIOReads        3
#define cDiskIOReads        5
#define nDiskIOLA1          7
#define cDiskIOLA1          9
#define nDiskIOWrites       4
#define cDiskIOWrites       6


// diskIODevice
class AGENTPP_DECL diskIODevice: public SnmpDisplayString{
public:
    diskIODevice(const Oidx &);
    virtual ~diskIODevice() {};
    virtual MibEntry * clone(){
        MibEntry * other = new diskIODevice(oid);
        ((diskIODevice*)other)->replace_value(value->clone());
        ((diskIODevice*)other)->set_reference_to_table(my_table);
        return other;
    };
    void get_request(Request* req, int ind);
    
};

// diskIOReads
class AGENTPP_DECL diskIOReads: public Counter32MibLeaf{
public:
    diskIOReads(const Oidx &);
    virtual ~diskIOReads() {};
    virtual MibEntry * clone(){
        MibEntry * other = new diskIOReads(oid);
        ((diskIOReads*)other)->replace_value(value->clone());
        ((diskIOReads*)other)->set_reference_to_table(my_table);
        return other;
    };
    void get_request(Request* req, int ind);
private:
    long get_disk_io_reads();
};

// diskIOWrites
class AGENTPP_DECL diskIOWrites: public Counter32MibLeaf{
public:
    diskIOWrites(const Oidx &);
    virtual ~diskIOWrites() {}
    virtual MibEntry * clone(){
        MibEntry * other = new diskIOWrites(oid);
        ((diskIOWrites*)other)->replace_value(value->clone());
        ((diskIOWrites*)other)->set_reference_to_table(my_table);
        return other;
    };
    void get_request(Request* req, int ind);
private:
    long get_disk_io_writes();
};

// diskIOLA1
class AGENTPP_DECL diskIOLA1: public MibLeaf{
public:
    diskIOLA1(const Oidx &);
    virtual ~diskIOLA1() {}
    void get_request(Request* req, int ind);
private:
    long get_disk_io_la1();
};

// diskIOEntry
class AGENTPP_DECL diskIOEntry: public MibTable{
public:
    diskIOEntry();
    virtual ~diskIOEntry();
    static diskIOEntry * instance;
    static diskIOEntry * get_instance(Mib * mib){
        Oidx oid(oidDiskIOEntry);
        diskIOEntry * entry = (diskIOEntry*)mib->get(oid);
        return (entry) ? entry : instance;
    }
    void set_row(MibTableRow * r, const Counter32 & rd, const Counter32 & wr, const SnmpInt32 & la);
    MibTableRow * add_entry(const OctetStr & name, const Counter32 & rd, const Counter32 & wr, const SnmpInt32 & la);
};

class AGENTPP_DECL ucd_diskio_mib: public MibGroup{
public:
    ucd_diskio_mib();
    virtual ~ucd_diskio_mib() { };
    virtual diskIOEntry* get_disk_IO_table(){
        ListCursor<MibEntry> content = get_content();
        return (diskIOEntry*)content.get();
    }
};

#ifdef AGENTPP_NAMESPACE
};
#endif

#endif