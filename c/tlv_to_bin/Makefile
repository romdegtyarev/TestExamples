default : test_xml_tlv.o
	gcc -std=c99 -o test_xml_tlv test_xml_tlv.o

test_xml_tlv.o : test_xml_tlv.c
	gcc -std=c99 --pedantic -c test_xml_tlv.c

clean :
	rm -f test_xml_tlv test_xml_tlv.o
