# net-snmp-utils for test
snmpwalk -v2c -cpublic localhost:4700 .1
snmptranslate -Tp .1
# snmpget -v2c -cpublic localhost:4700 
# snmpgetnext -v2c -cpublic localhost:4700 