#include <if_mib.h>

static const char * loggerModuleName = "agent++.if_mib";

#if defined(AGENTPP_NAMESPACE)
using namespace Agentpp;
#endif

// ifName
ifName::ifName(const Oidx & o): SnmpDisplayString(o, READONLY, new OctetStr()){

}

void ifName::get_request(Request * req, int ind){
    MibLeaf::get_request(req, ind);
}

// ifHCInOctets
ifHCInOctets::ifHCInOctets(const Oidx & o): MibLeaf(o, READONLY, new Counter64()){

}

void ifHCInOctets::get_request(Request * req, int ind){
    my_row->get_nth(0)->get_value(itf);
    Counter64 in = get_if_hc_in_octets();
    set_value(in);
    MibLeaf::get_request(req, ind);
}

unsigned long long ifHCInOctets::get_if_hc_in_octets(){

    return 0;
}

// ifHCInOctets
ifHCOutOctets::ifHCOutOctets(const Oidx & o): MibLeaf(o, READONLY, new Counter64()){

}

void ifHCOutOctets::get_request(Request * req, int ind){
    my_row->get_nth(0)->get_value(itf);
    Counter64 out = get_if_hc_out_octets();
    set_value(out);
    MibLeaf::get_request(req, ind);
}

unsigned long long ifHCOutOctets::get_if_hc_out_octets(){

    return 0;
}
// ifXEntry
ifXEntry* ifXEntry::instance = NULL; 

const index_info indIfXEntry[1] = {
    {sNMP_SYNTAX_INT, false, 1, 1}
};

ifXEntry::ifXEntry() : MibTable(oidIfXEntry, indIfXEntry, 1){
    instance = this;
    add_col(new ifName(colIfName));
    add_col(new ifHCInOctets(colIfHCInOctets));
    add_col(new ifHCOutOctets(colIfHCOutOctets));
    add_row("1");
    add_row("2");
}

ifXEntry::~ifXEntry(){
    instance = NULL;
}

if_mib::if_mib(): MibGroup(oidIf, "if_mib"){
    add(new ifXEntry());
}