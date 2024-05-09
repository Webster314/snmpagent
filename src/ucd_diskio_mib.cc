
#include <ucd_diskio_mib.h>

static const char * loggerModuleName = "agent++.ucd_diskio_mib";

#if defined(AGENTPP_NAMESPACE)
using namespace Agentpp;
#endif

// diskIOReads
diskIOReads::diskIOReads(const Oidx & o) : Counter32MibLeaf(o) {

}

void diskIOReads::get_request(Request * req, int ind){
    Counter32 rd = get_disk_io_reads();
    set_value(rd);
    MibLeaf::get_request(req, ind);
}

long diskIOReads::get_disk_io_reads(){
    FILE * fp;
    char * value;
    char line[128];
    fp = fopen("/sys/block/sda/stat", "r");
    if(fp == NULL){
        LOG_BEGIN(loggerModuleName, ERROR_LOG | 1);
        LOG("UCD_DISKIO_MIB: /sys/block/sda/stat open failed (get_disk_io_reads())");
        LOG(errno);
        LOG(strerror(errno));
        LOG_END;
        return 0;
    }
    long rd;
    fgets(line, sizeof(line), fp);
    value = strtok(line, " \n");
    for(int i = 1; i < offDiskIOReads; i++){
        value = strtok(NULL, " \n");
    }
    rd = atol(value);
    fclose(fp);
    fp = NULL;
    return rd;
}

// diskIOWrites
diskIOWrites::diskIOWrites(const Oidx & o) : Counter32MibLeaf(o) {

}

void diskIOWrites::get_request(Request * req, int ind){
    Counter32 wr = get_disk_io_writes();
    set_value(wr);
    MibLeaf::get_request(req, ind);
}

long diskIOWrites::get_disk_io_writes(){
    FILE * fp;
    char * value;
    char line[128];
    fp = fopen("/sys/block/sda/stat", "r");
    if(fp == NULL){
        LOG_BEGIN(loggerModuleName, ERROR_LOG | 1);
        LOG("UCD_DISKIO_MIB: /sys/block/sda/stat open failed (get_disk_io_writes())");
        LOG(errno);
        LOG(strerror(errno));
        LOG_END;
        return 0;
    }
    long wr;
    fgets(line, sizeof(line), fp);
    value = strtok(line, " \n");
    for(int i = 1; i < offDiskIOWrites; i++){
        value = strtok(NULL, " \n");
    }
    wr = atol(value);
    fclose(fp);
    fp = NULL;
    return wr;
}

// diskIOLA1
diskIOLA1::diskIOLA1(const Oidx & o) : MibLeaf(o, READONLY, new SnmpInt32()) {

}

void diskIOLA1::get_request(Request* req, int ind){
    SnmpInt32 la1 = get_disk_io_la1();
    set_value(la1);
    MibLeaf::get_request(req, ind);
}

long diskIOLA1::get_disk_io_la1(){
    return 1;
}

// diskIOEntry
diskIOEntry* diskIOEntry::instance = NULL;

const index_info indDiskIOEntry[1] = {
    {sNMP_SYNTAX_INT, false, 1, 1}
};

diskIOEntry::diskIOEntry(): MibTable(oidDiskIOEntry, indDiskIOEntry, 1){
    instance = NULL;
    add_col(new diskIOReads(colDiskIOReads));
    add_col(new diskIOWrites(colDiskIOWrites));
    add_col(new diskIOLA1(colDiskIOLA1));
    MibTableRow * r = add_row("10");
    set_row(r, 0, 0, 0);
    // add_entry("10", 0, 0, 0);
}

diskIOEntry::~diskIOEntry(){
    instance = NULL;
}

void diskIOEntry::set_row(MibTableRow * r, const Counter32 & p0, const Counter32 & p1, const SnmpInt32 & p2){
    r->get_nth(0)->replace_value(new Counter32(p0));
    r->get_nth(1)->replace_value(new Counter32(p1));
    r->get_nth(2)->replace_value(new SnmpInt32(p2));
}

MibTableRow * diskIOEntry::add_entry(const OctetStr & name, const Counter32 & rd, const Counter32 & wr, const SnmpInt32 & la){
    Oidx index = name.get_printable();
    start_synch();
    MibTableRow * r = find_index(index);
    if(r){
        LOG_BEGIN(loggerModuleName, ERROR_LOG | 1);
        LOG("UCD_DISKIO_MIB: ");
        LOG(index.get_printable());
        LOG("entry exists in MibTable (diskIOEntry::add_entry())");
        LOG_END;
        end_synch();
        return r;
    }
    r = add_row(index);
    set_row(r, rd, wr, la);
    end_synch();
    return r;
}

// ucd_diskio_mib
ucd_diskio_mib::ucd_diskio_mib() : MibGroup(oidUcdDiskIO, "ucd_diskio_mib"){
    add(new diskIOEntry()); 
}