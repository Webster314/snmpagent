#include <ucd_snmp_mib.h>

static const char * loggerModuleName = "agent++.ucd_snmp_mib";

#if defined(AGENTPP_NAMESPACE)
using namespace Agentpp;
#endif

// interrupts
ssRawInterrupts::ssRawInterrupts() : Counter32MibLeaf(oidSsRawInterrupts) {

}

void ssRawInterrupts::get_request(Request * req, int ind){
    Counter32 intr = get_interrupt();
    set_value(intr);
    MibLeaf::get_request(req, ind);
}

long ssRawInterrupts::get_interrupt(){
    FILE * fp;
    char * key, * value;
    char line[128];
    fp = fopen("/proc/stat", "r");
    if(fp == NULL){
        LOG_BEGIN(loggerModuleName, ERROR_LOG | 1);
        LOG("UCD_SNMP_MIB: /proc/stat open failed (get_interrupt())");
        LOG(errno);
        LOG(strerror(errno));
        LOG_END;
    }
    long intr;
    while (fgets(line, sizeof(line), fp)) {
        if ((key = strtok(line, " \n")) && (value = strtok(NULL, " \n"))) {
            if (strcmp(key, "intr") == 0) {
                intr = atol(value);
                break;
            }
        }
    }   
    fclose(fp);
    fp = NULL;
    return intr;
}

// context switches
ssRawContexts::ssRawContexts() : Counter32MibLeaf(oidSsRawContexts){

}
void ssRawContexts::get_request(Request * req, int ind){
    Counter32 ctxt = get_context_switch();
    set_value(ctxt);
    MibLeaf::get_request(req, ind);
}

long ssRawContexts::get_context_switch(){
    FILE * fp;
    char * key, * value;
    char line[128];
    fp = fopen("/proc/stat", "r");
    if(fp == NULL){
        LOG_BEGIN(loggerModuleName, ERROR_LOG | 1);
        LOG("UCD_SNMP_MIB: /proc/stat open failed (get_context_switch())");
        LOG(errno);
        LOG(strerror(errno));
        LOG_END;
    }
    long ctxt;
    while (fgets(line, sizeof(line), fp)) {
        if ((key = strtok(line, " \n")) && (value = strtok(NULL, " \n"))) {
            if (strcmp(key, "ctxt") == 0) {
                ctxt = atol(value);
                break;
            }
        }
    }   
    fclose(fp);
    fp = NULL;
    return ctxt;
}

// cpu timeinfo
cpuTime::cpuTime(const Oidx & o, const Cputimetype & t): Counter32MibLeaf(o), type(t) {

}

long cpuTime::get_cpu_time(){
    FILE * fp;
    char * value;
    char line[128];
    fp = fopen("/proc/stat", "r");
    if(fp == NULL){
        LOG_BEGIN(loggerModuleName, ERROR_LOG | 1);
        LOG("UCD_SNMP_MIB: /proc/stat open failed (get_cpu_time())");
        LOG(errno);
        LOG(strerror(errno));
        LOG_END;
    }
    long cpu_time;
    fgets(line, sizeof(line), fp);
    strtok(line, " \n");
    for(unsigned int i = 0; i <= type; i++){
        value = strtok(NULL, " \n");
    }
    cpu_time = atol(value);
    fclose(fp);
    fp = NULL;
    return cpu_time;
}

void cpuTime::get_request(Request * req, int ind){
    Counter32 tm = get_cpu_time();
    set_value(tm);
    MibLeaf::get_request(req, ind);
}

// mem
memInfo::memInfo(const Oidx & o, const OctetStr & i): MibLeaf(o, READONLY, new SnmpInt32(0)), info(i){

}

long memInfo::get_mem_info(){
    FILE * fp;
    char * key, * value;
    char line[128];
    fp = fopen("/proc/meminfo", "r");
    if(fp == NULL){
        LOG_BEGIN(loggerModuleName, ERROR_LOG | 1);
        LOG("UCD_SNMP_MIB: /proc/meminfo open failed (get_mem_info())");
        LOG(errno);
        LOG(strerror(errno));
        LOG_END;
    }
    long mem_info;
    while (fgets(line, sizeof(line), fp)) {
        if ((key = strtok(line, ": \n")) && (value = strtok(NULL, ": \n"))) {
            if (strcmp(key, (char*)info.data()) == 0) {
                mem_info = atol(value);
                break;
            }
        }
    }
    fclose(fp);
    fp = NULL;
    return mem_info;    // KiB
} 

void memInfo::get_request(Request * req, int ind){
    SnmpInt32 mem_info = get_mem_info();
    set_value(mem_info);
    MibLeaf::get_request(req, ind);
}

memTotalReal::memTotalReal(): MibLeaf(oidMemTotalReal, READONLY, new SnmpInt32(0)){

}

long memTotalReal::get_mem_total_real(){
    long pages;
    long page_size;
    pages = sysconf(_SC_PHYS_PAGES);
    if(pages == -1){
        LOG_BEGIN(loggerModuleName, ERROR_LOG | 1);
        LOG("UCD_SNMP_MIB: mem pages failed (sysconf)");
        LOG(errno);
        LOG(strerror(errno));
        LOG_END;
    }
    page_size = sysconf(_SC_PAGE_SIZE);
    if(page_size == -1){
        LOG_BEGIN(loggerModuleName, ERROR_LOG | 1);
        LOG("UCD_SNMP_MIB: mem pagesize filed (sysconf)");
        LOG(errno);
        LOG(strerror(errno));
        LOG_END;
    }
    long mem_total = pages * page_size;
    return mem_total / 1024;
}

void memTotalReal::get_request(Request * req, int ind){
    SnmpInt32 mem_total(get_mem_total_real());
    set_value(mem_total);
    MibLeaf::get_request(req, ind);
}

// laLoadTable
// laIndex
laIndex::laIndex(const Oidx & o): MibLeaf(o, READONLY,  new SnmpInt32()){

}
// laName
laName::laName(const Oidx & o): SnmpDisplayString(o, READONLY, new OctetStr()){

}

laName::laName(const Oidx & o, const OctetStr & s): SnmpDisplayString(o, READONLY, new OctetStr(s)){

}

void laName::get_request(Request * req, int ind){
    MibLeaf::get_request(req, ind);
}

// laLoad
laLoad::laLoad(const Oidx & o): SnmpDisplayString(o, READONLY, new OctetStr()){

}

laLoad::laLoad(const Oidx & o, int min): SnmpDisplayString(o, READONLY, new OctetStr()), minute(min){

}

void laLoad::get_request(Request * req, int ind){
    Oidx row = req->get_oid(ind);
    int r = atoi(row.cut_left(row.len()-1).get_printable());
    switch(r){
        case 1 : minute = 1; break;
        case 2 : minute = 5; break;
        case 3 : minute = 15; break;
    }
    OctetStr load = get_laLoad();
    set_value(load);
    MibLeaf::get_request(req, ind);
}

OctetStr laLoad::get_laLoad(){
    FILE * fp;
    char * value = "";
    char line[128];
    fp = fopen("/proc/loadavg", "r");
    if(fp == NULL){
        LOG_BEGIN(loggerModuleName, ERROR_LOG | 1);
        LOG("UCD_SNMP_MIB: /proc/loadavg open failed (get_laload())");
        LOG(errno);
        LOG(strerror(errno));
        LOG_END;
    }
    fgets(line, sizeof(line), fp);
    switch(minute){
        case 15:value = strtok(line, " \n");
                value = strtok(NULL, " \n");
                value = strtok(NULL, " \n"); break;
        case 5: value = strtok(line, " \n");
                value = strtok(NULL, " \n"); break;
        case 1: value = strtok(line, " \n"); break;
    }
    // std::cout << OctetStr(value).get_printable();
    fclose(fp);
    fp = NULL;
    return OctetStr(value);
}


// laEntry

laEntry* laEntry::instance = NULL;

const index_info indLaEntry[1] = {
    {sNMP_SYNTAX_INT, false, 1, 1}
};

laEntry::laEntry() : MibTable(oidLaEntry, indLaEntry, 1) {
    instance = this;
    add_col(new laIndex(colLaIndex));
    add_col(new laName(colLaName));
    add_col(new laLoad(colLaLoad));
    MibTableRow * la1 = add_row("1");
    set_row(la1, 1, "Load-1", "0");
    MibTableRow * la5 = add_row("2");
    set_row(la5, 2, "Load-5", "0");
    MibTableRow * la15 = add_row("3");
    set_row(la15, 3, "Load-15", "0");
}

laEntry::~laEntry(){
    instance = NULL;
}

void laEntry::set_row(MibTableRow * r, const SnmpInt32 & p0, const OctetStr & p1, const OctetStr & p2){
    r->get_nth(0)->replace_value(new SnmpInt32(p0));
    r->get_nth(1)->replace_value(new OctetStr(p1));
    r->get_nth(2)->replace_value(new OctetStr(p2));
}

MibTableRow * laEntry::add_entry(const OctetStr & ind, const OctetStr & name, const OctetStr & load){
    Oidx index = Oidx::from_string(name, FALSE);
    start_synch();
    MibTableRow * r = find_index(index);
    if(r){
        LOG_BEGIN(loggerModuleName, ERROR_LOG | 1);
        LOG("UCD_SNMP_MIB: ");
        LOG(index.get_printable());
        LOG("entry exists in MibTable (laEntry::add_entry())");
        LOG_END;
        end_synch();
        return r;
    }
    r = add_row(index);
    end_synch();
    return r;   
}

// dskTable
// dskIndex
dskIndex::dskIndex(const Oidx & o): MibLeaf(o, READONLY,  new SnmpInt32()){

}
// dskPath
dskPath::dskPath(const Oidx & o) : SnmpDisplayString(o, READONLY, new OctetStr()){

}

dskPath::dskPath(const Oidx & o, const OctetStr & s) : SnmpDisplayString(o, READONLY, new OctetStr(s)){

}

void dskPath::get_request(Request * req, int ind){
    MibLeaf::get_request(req, ind);
}

// dskPercentNode

dskPercentNode::dskPercentNode(const Oidx & o) : MibLeaf(o, READONLY, new SnmpInt32()){

}

dskPercentNode::dskPercentNode(const Oidx & o, const OctetStr & m) : MibLeaf(o, READONLY, new SnmpInt32()), mnt(m){

}


void dskPercentNode::get_request(Request * req, int ind){
    mnt = my_row->get_nth(0)->get_value().get_printable_value();
    SnmpInt32 percent(get_dsk_percent_node());
    set_value(percent);
    MibLeaf::get_request(req, ind);
}
long dskPercentNode::get_dsk_percent_node(const char * m){
    struct statfs fs;
    if(statfs(m, &fs) == -1){
        LOG_BEGIN(loggerModuleName, ERROR_LOG | 1);
        LOG("UCD_SNMP_MIB: statfs() failed (get_dsk_percent_node())");
        LOG(errno);
        LOG(strerror(errno));
        LOG_END;
        return 0;
    }
    long total = fs.f_files;
    long free = fs.f_ffree;
    if(total == 0){
        return 0;
    }
    long freep = (1.0 - ((double)free / (double)total)) * 100;
    long percent = freep;
    return percent;
}
long dskPercentNode::get_dsk_percent_node(){
    struct statfs fs;
    if(statfs(mnt.get_printable(), &fs) == -1){
        LOG_BEGIN(loggerModuleName, ERROR_LOG | 1);
        LOG("UCD_SNMP_MIB: statfs() failed (get_dsk_percent_node())");
        LOG(errno);
        LOG(strerror(errno));
        LOG_END;
        return 0;
    }
    long total = fs.f_files;
    long free = fs.f_ffree;
    if(total == 0){
        return 0;
    }
    long freep = (1.0 - ((double)free / (double)total)) * 100;
    long percent = freep;
    return percent;
}

// dskEntry

dskEntry * dskEntry::instance = NULL;

const index_info indDskEntry[1] = {
    {sNMP_SYNTAX_INT, false, 1, 1}
};

dskEntry::dskEntry(): MibTable(oidDskEntry, indDskEntry, 1){
    instance = this;
    add_col(new dskPath(colDskPath));
    add_col(new dskPercentNode(colDskPercentNode));
    MibTableRow * r;
    r = add_row("1");
    set_row(r, "/", 0);
    r = add_row("2");
    set_row(r, "/boot/efi", 0);
}

dskEntry::~dskEntry(){
    instance = NULL;
}

void dskEntry::set_row(MibTableRow * r, const OctetStr & name, const SnmpInt32 & percent){
    r->get_nth(0)->replace_value(new OctetStr(name));
    r->get_nth(1)->replace_value(new SnmpInt32(percent));
}
ucd_snmp_mib::ucd_snmp_mib() : MibGroup(oidUcdSnmp, "ucd_snmp_mib") {
    // cpu
    add(new ssRawContexts());
    add(new ssRawInterrupts());
    add(new cpuTime(oidSsCpuRawUser,        offUserTime));
    add(new cpuTime(oidSsCpuRawNice,        offNiceTime));
    add(new cpuTime(oidSsCpuRawSystem,      offSystemTime));
    add(new cpuTime(oidSsCpuRawIdle,        offIdleTime));
    add(new cpuTime(oidSsCpuRawWait,        offIowaitTime));
    // add(new cpuTime(oidSsCpuRawKernel,      ));
    add(new cpuTime(oidSsCpuRawInterrupt,   offIrqTime));
    add(new cpuTime(oidSsCpuRawSoftIRQ,     offSoftIrqTime));
    add(new cpuTime(oidSsCpuRawSteal,       offStealTime));
    add(new cpuTime(oidSsCpuRawGuest,       offGuestTime));
    add(new cpuTime(oidSsCpuRawGuestNice,   offGuestNiceTime));

    // mem
    add(new memInfo(oidMemTotalSwap, "SwapTotal"));
    add(new memInfo(oidMemAvailSwap, "SwapFree"));
    add(new memInfo(oidMemTotalReal, "MemTotal"));
    add(new memInfo(oidMemAvailReal, "MemFree"));
    add(new memInfo(oidMemShared,    "Shmem"));
    add(new memInfo(oidMemCached,    "Cached"));
    add(new memInfo(oidMemBuffer,    "Buffers"));
    // add(new memTotalReal());

    // laTable
    add(new laEntry());
    // dskTable
    add(new dskEntry());
}