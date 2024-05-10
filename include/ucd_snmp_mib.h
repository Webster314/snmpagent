#ifndef _ucd_snmp_mib_h_
#define _ucd_snmp_mib_h_

#include <libagent.h>
#include <sys/vfs.h>
#include <agent_pp/mib.h>
#include <agent_pp/snmp_textual_conventions.h>

#define oidUcdSnmp              "1.3.6.1.4.1.2021"
// Scalars

// cpu
#define oidSsCpuRawUser	        "1.3.6.1.4.1.2021.11.50.0"
#define oidSsCpuRawNice	        "1.3.6.1.4.1.2021.11.51.0"
#define oidSsCpuRawSystem	    "1.3.6.1.4.1.2021.11.52.0"
#define oidSsCpuRawIdle         "1.3.6.1.4.1.2021.11.53.0"
#define oidSsCpuRawWait         "1.3.6.1.4.1.2021.11.54.0"
#define oidSsCpuRawKernel       "1.3.6.1.4.1.2021.11.55.0"
#define oidSsCpuRawInterrupt    "1.3.6.1.4.1.2021.11.56.0"
#define oidSsRawInterrupts      "1.3.6.1.4.1.2021.11.59.0"
#define oidSsRawContexts	    "1.3.6.1.4.1.2021.11.60.0"
#define oidSsCpuRawSoftIRQ		"1.3.6.1.4.1.2021.11.61.0"
#define oidSsCpuRawSteal		"1.3.6.1.4.1.2021.11.64.0"
#define oidSsCpuRawGuest        "1.3.6.1.4.1.2021.11.65.0"
#define oidSsCpuRawGuestNice    "1.3.6.1.4.1.2021.11.66.0"

// mem
#define oidMemTotalSwap         "1.3.6.1.4.1.2021.4.3.0"
#define oidMemAvailSwap         "1.3.6.1.4.1.2021.4.4.0"
#define oidMemTotalReal         "1.3.6.1.4.1.2021.4.5.0"
#define oidMemAvailReal         "1.3.6.1.4.1.2021.4.6.0"
#define oidMemShared            "1.3.6.1.4.1.2021.4.13.0"
#define oidMemBuffer            "1.3.6.1.4.1.2021.4.14.0"
#define oidMemCached            "1.3.6.1.4.1.2021.4.15.0"

typedef unsigned int Cputimetype;
#define offUserTime             0
#define offNiceTime             1
#define offSystemTime           2
#define offIdleTime             3
#define offIowaitTime           4
#define offIrqTime              5
#define offSoftIrqTime          6
#define offStealTime            7
#define offGuestTime            8
#define offGuestNiceTime        9

// Columns
#define oidLaIndex              "1.3.6.1.4.1.2021.10.1.1"
#define colLaIndex              "1"
#define oidLaName               "1.3.6.1.4.1.2021.10.1.2"
#define colLaName               "2"
#define oidLaLoad               "1.3.6.1.4.1.2021.10.1.3"
#define colLaLoad               "3"

#define oidDskPath              "1.3.6.1.4.1.2021.9.1.2"
#define colDskPath              "2"
#define oidDskPercentNode       "1.3.6.1.4.1.2021.9.1.10"
#define colDskPercentNode       "10"


// Tables
#define oidLaTable              "1.3.6.1.4.1.2021.10"
#define oidLaEntry              "1.3.6.1.4.1.2021.10.1"
#define nLaIndex                0
#define cLaIndex                1
#define nLaName                 1                
#define cLaName                 2
#define nLaLoad                 2              
#define cLaLoad                 3

#define oidDskTable             "1.3.6.1.4.1.2021.9"
#define oidDskEntry             "1.3.6.1.4.1.2021.9.1"
#define nDskPath                0
#define cDskPath                2
#define nDskPercentNode         8
#define cDskPercentNode         10


#ifdef AGENTPP_NAMESPACE
using namespace Agentpp;
#endif

// cpu
class AGENTPP_DECL ssRawInterrupts: public Counter32MibLeaf {
public:
    ssRawInterrupts();
    virtual ~ssRawInterrupts() { };
    void get_request(Request * req, int ind);
private:
    long get_interrupt();
};
class AGENTPP_DECL ssRawContexts: public Counter32MibLeaf {
public:
    ssRawContexts();
    virtual ~ssRawContexts() { };
    void get_request(Request *, int);
private:
    long get_context_switch();
};

class AGENTPP_DECL cpuTime: public Counter32MibLeaf {
public:
    cpuTime(const Oidx&, const Cputimetype &);
    virtual ~cpuTime() { };
    void get_request(Request *, int);
    Cputimetype type;
private:
    long get_cpu_time();
};

class AGENTPP_DECL memInfo: public MibLeaf{
public:
    memInfo(const Oidx&, const OctetStr&);
    virtual ~memInfo() { };
    void get_request(Request *, int);
    OctetStr info;
private:
    long get_mem_info();    
};

class AGENTPP_DECL memTotalReal: public MibLeaf{
public:
    memTotalReal();
    virtual ~memTotalReal() { };
    void get_request(Request *, int);
private:
    long get_mem_total_real(); 
};

// laTable

// laIndex
class AGENTPP_DECL laIndex: public MibLeaf{
public:
    laIndex(const Oidx &);
    virtual ~laIndex() { };
    virtual MibEntry* clone(){
        MibEntry * other = new laIndex(oid);
        ((laIndex*)other)->replace_value(value->clone());
        ((laIndex*)other)->set_reference_to_table(my_table);
        return other;
    }   
};
// laName 
class AGENTPP_DECL laName: public SnmpDisplayString{
public:
    laName(const Oidx &);
    laName(const Oidx &, const OctetStr &);
    virtual ~laName() { }
    virtual MibEntry* clone(){
        MibEntry * other = new laName(oid);
        ((laName*)other)->replace_value(value->clone());
        ((laName*)other)->set_reference_to_table(my_table);
        return other;
    }
    void get_request(Request *, int);
};

// laLoad
class AGENTPP_DECL laLoad: public SnmpDisplayString{
public:
    laLoad(const Oidx&);
    laLoad(const Oidx&, int);
    virtual ~laLoad() {};
    virtual MibEntry* clone(){
        MibEntry * other = new laLoad(oid, minute);
        ((laLoad*)other)->replace_value(value->clone());
        ((laLoad*)other)->set_reference_to_table(my_table);
        return other;
    }
    void get_request(Request *, int);
private:
    int minute;
    OctetStr get_laLoad();
};

//laEntry
class AGENTPP_DECL laEntry: public MibTable{
public:
    laEntry();
    virtual ~laEntry();
    static laEntry* instance; 
    static laEntry* get_instance(Mib* mib){
        Oidx oid(oidLaEntry);
        laEntry * entry = (laEntry*)mib->get(oid);
        return (entry) ? entry : instance;
    }
    void set_row(MibTableRow * r, const SnmpInt32 & index, const OctetStr & name, const OctetStr & load);
    MibTableRow * add_entry(const OctetStr & ind, const OctetStr & name, const OctetStr & load);
};

// dskTable
// dskIndex
class AGENTPP_DECL dskIndex: public MibLeaf{
public:
    dskIndex(const Oidx &);
    virtual ~dskIndex() { };
    virtual MibEntry* clone(){
        MibEntry * other = new dskIndex(oid);
        ((dskIndex*)other)->replace_value(value->clone());
        ((dskIndex*)other)->set_reference_to_table(my_table);
        return other;
    }
};

// dskPath
class AGENTPP_DECL dskPath: public SnmpDisplayString{
public:
    dskPath(const Oidx &);
    dskPath(const Oidx &, const OctetStr &);
    virtual ~dskPath() { }
    virtual MibEntry* clone(){
        MibEntry * other = new dskPath(oid);
        ((dskPath*)other)->replace_value(value->clone());
        ((dskPath*)other)->set_reference_to_table(my_table);
        return other;
    }
    void get_request(Request *, int);
};

// dskPercentNode
class AGENTPP_DECL dskPercentNode: public MibLeaf{
public:
    dskPercentNode(const Oidx &);
    dskPercentNode(const Oidx &, const OctetStr & m);
    virtual ~dskPercentNode() {};
    virtual MibEntry* clone(){
        MibEntry * other = new dskPercentNode(oid, mnt);
        ((dskPercentNode*)other)->replace_value(value->clone());
        ((dskPercentNode*)other)->set_reference_to_table(my_table);
        return other;
    }
    void get_request(Request *, int);
    OctetStr mnt;
private:
    long get_dsk_percent_node();
    long get_dsk_percent_node(const char *);
};

// dskEntry
class AGENTPP_DECL dskEntry: public MibTable{
public:
    dskEntry();
    virtual ~dskEntry();
    static dskEntry* instance;
    static dskEntry* get_instance(Mib* mib){
        Oidx oid(oidDskEntry);
        dskEntry * entry = (dskEntry*)mib->get(oid);
        return (entry) ? entry : instance;
    }
    void set_row(MibTableRow * r, const OctetStr & name, const SnmpInt32 & percent);
    MibTableRow * add_entry(const OctetStr & ind, const OctetStr & name, const OctetStr & percent);
};



/**********************************************************************
 *
 *  class ucd_snmp_mib 
 *
 **********************************************************************/

class AGENTPP_DECL ucd_snmp_mib: public MibGroup {
public:
    ucd_snmp_mib();
    virtual ~ucd_snmp_mib() { };
};

#endif