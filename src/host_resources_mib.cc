#include <host_resources_mib.h>

static const char * loggerModuleName = "agent++.host_resources_mib";

#if defined(AGENTPP_NAMESPACE)
using namespace Agentpp;
#endif

// hrSystemUptime
hrSystemUptime::hrSystemUptime(): MibLeaf(oidHrSystemUptime, READONLY, new TimeTicks(0)){

}

long hrSystemUptime::get_hr_system_uptime(){
    FILE *fp;
    float uptime;
    char buffer[1024];
    fp = fopen("/proc/uptime", "r");
    if (fp == NULL) {
        LOG_BEGIN(loggerModuleName, 1);
        LOG("HOST_RESOURCES_MIB: /proc/uptime open failed (get_hr_system_uptime())");
        LOG(errno);
        LOG(strerror(errno));
        LOG_END;
    }
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        uptime = strtof(buffer, NULL);
    }
    fclose(fp);
    return uptime*100;
}

void hrSystemUptime::get_request(Request * req, int ind){
    TimeTicks tm = get_hr_system_uptime();
    set_value(tm);
    MibLeaf::get_request(req, ind);
}

// hrProcessorFrwid

hrProcessorFrwID::hrProcessorFrwID(const Oidx & o): MibLeaf(o, READONLY, new Oidx("0.0")) {

}

// hrProcessorLoad
hrProcessorLoad::hrProcessorLoad(const Oidx & o): MibLeaf(o, READONLY, new SnmpInt32(0)) {

}

// hrProcessorEntry
hrProcessorEntry * hrProcessorEntry::instance = NULL;

const index_info  indHrProcessorEntry[1] = {
    {sNMP_SYNTAX_INT, false, 1, 1}
};

hrProcessorEntry::hrProcessorEntry() : MibTable(oidHrProcessorEntry, indHrProcessorEntry, 1) {
    // index : hrProcessorFrwID | HrProcessorLoad
    instance = this;
    add_col(new hrProcessorFrwID(colHrProcessorFrwID));
    add_col(new hrProcessorLoad(colHrProcessorLoad));
    char buf[16];
    for(int i = 0; i < 4; i++){
        sprintf(buf, "%d", i);
        add_row(buf);
    }
}

hrProcessorEntry::~hrProcessorEntry(){
    instance = NULL;
}

void hrProcessorEntry::set_row(MibTableRow * r, const Oidx & p0, const SnmpInt32 & p1){
    r->get_nth(0)->replace_value(new Oidx(p0));
    r->get_nth(1)->replace_value(new SnmpInt32(p1));
}

MibTableRow * hrProcessorEntry::add_entry(const OctetStr & name, const Oidx & frwid, const SnmpInt32 & load){
    Oidx index = Oidx::from_string(name, FALSE);
    start_synch();
    MibTableRow * r = find_index(index);
    if(r){
        LOG_BEGIN(loggerModuleName, ERROR_LOG | 1);
        LOG("HOST_RESOURCES_MIB: ");
        LOG(index.get_printable());
        LOG("exists in MibTable (hrPorcessorEntry::add_entry)");
        LOG_END;
        end_synch();
        return r;
    }
    r = add_row(index);
    set_row(r, frwid, load);
    end_synch();
    return r;
}

// hrStorageSize
hrStorageSize::hrStorageSize(const Oidx & o) : MibLeaf(o, READWRITE, new SnmpInt32()){

}

hrStorageSize::hrStorageSize(const Oidx & o, const OctetStr & m) : MibLeaf(o, READWRITE, new SnmpInt32()), mnt(m){

}

void hrStorageSize::get_request(Request * req, int ind){
    mnt = "/";
    SnmpInt32 size = get_hr_storage_size();
    set_value(size);
    MibLeaf::get_request(req, ind);
}

long hrStorageSize::get_hr_storage_size(){
    struct statfs fs;
    if(statfs(mnt.get_printable(), &fs) == -1){
        LOG_BEGIN(loggerModuleName, ERROR_LOG | 1);
        LOG("HOST_RESOURCES_MIB: statfs() failed (get_dsk_percent_node())");
        LOG(errno);
        LOG(strerror(errno));
        LOG_END;
        return 0;
    }
    long blocks = fs.f_blocks;
    long fssize = blocks;
    return fssize;
}

// hrStorageUsed
hrStorageUsed::hrStorageUsed(const Oidx & o) : MibLeaf(o, READONLY, new SnmpInt32()){

}

hrStorageUsed::hrStorageUsed(const Oidx & o, const OctetStr & m) : MibLeaf(o, READONLY, new SnmpInt32()), mnt(m){

}

void hrStorageUsed::get_request(Request * req, int ind){
    mnt = "/";
    SnmpInt32 used = get_hr_storage_used();
    set_value(used);
    MibLeaf::get_request(req, ind);
}

long hrStorageUsed::get_hr_storage_used(){
    struct statfs fs;
    if(statfs(mnt.get_printable(), &fs) == -1){
        LOG_BEGIN(loggerModuleName, ERROR_LOG | 1);
        LOG("HOST_RESOURCES_MIB: statfs() failed (get_hr_storage_used())");
        LOG(errno);
        LOG(strerror(errno));
        LOG_END;
        return 0;
    }
    long used = fs.f_blocks - fs.f_bfree;
    long fssize = used;
    return fssize;
}

// hrStorageEntry
hrStorageEntry * hrStorageEntry::instance = NULL;

const index_info  indHrStorageEntry[1] = {
    {sNMP_SYNTAX_INT, false, 1, 1}
};

hrStorageEntry::hrStorageEntry() : MibTable(oidHrStorageEntry, indHrStorageEntry, 1){
    instance = this;
    add_col(new hrStorageSize(colHrStorageSize));
    add_col(new hrStorageUsed(colHrStorageUsed));
    MibTableRow * r;
    r = add_row("36");
    set_row(r, 0, 0);
    r = add_row("82");
    set_row(r, 0, 0);
}

hrStorageEntry::~hrStorageEntry(){
    instance = NULL;
}

void hrStorageEntry::set_row(MibTableRow * r, const SnmpInt32 & p0, const SnmpInt32 & p1){
    r->get_nth(0)->replace_value(new SnmpInt32(p0));
    r->get_nth(1)->replace_value(new SnmpInt32(p1));
}

MibTableRow* hrStorageEntry::add_entry(const OctetStr & ind, const SnmpInt32 & size, const SnmpInt32 & used){
    return 0;
}
// host_resources_mib
host_resources_mib::host_resources_mib() : MibGroup(oidHostResources, "host_resources_mib"){
    add(new hrSystemUptime());
    add(new hrProcessorEntry());
    add(new hrStorageEntry());
}