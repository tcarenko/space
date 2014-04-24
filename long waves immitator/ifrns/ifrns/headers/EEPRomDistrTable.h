/*
 * EEPRomDistrTable.h
 *
 * Created: 18.11.2011 17:01:35
 *  Author: Admin
 */ 


#ifndef EEPROMDISTRTABLE_H_
#define EEPROMDISTRTABLE_H_
//память
//размещение структуры диаграммы в EEPROM

#define DIAGRAM_STRUCT_EEPROM_ADDRESS				(0x0005)
#define DIAGRAM_STRUCT_EEPROM_CRC					(DIAGRAM_STRUCT_EEPROM_ADDRESS + sizeof(DiagramStruct) + 2)
#define SERVICE_STRUCT_EEPROM_ADDRESS				(DIAGRAM_STRUCT_EEPROM_CRC + 8)
#define SERVICE_STRUCT_EEPROM_CRC					(SERVICE_STRUCT_EEPROM_ADDRESS + sizeof(ServiceStruct)+2)

//#define DIAGRAM_STRUCT_Tpack_MSW_MSB_ADDRESS_EEPROM	(0x0005) //diagramStruct.Tpack
//#define DIAGRAM_STRUCT_Tpack_MSW_LSB_ADDRESS_EEPROM	(0x0006) //diagramStruct.Tpack
//#define DIAGRAM_STRUCT_Tpack_LSW_MSB_ADDRESS_EEPROM	(0x0007) //diagramStruct.Tpack
//#define DIAGRAM_STRUCT_Tpack_LSW_LSB_ADDRESS_EEPROM	(0x0008) //diagramStruct.Tpack
//
//#define DIAGRAM_STRUCT_tau_MSW_MSB_ADDRESS_EEPROM	(0x0009) //diagramStruct.tau
//#define DIAGRAM_STRUCT_tau_MSW_LSB_ADDRESS_EEPROM	(0x000A) //diagramStruct.tau
//#define DIAGRAM_STRUCT_tau_LSW_MSB_ADDRESS_EEPROM	(0x000B) //diagramStruct.tau
//#define DIAGRAM_STRUCT_tau_LSW_LSB_ADDRESS_EEPROM	(0x000C) //diagramStruct.tau
//
//#define DIAGRAM_STRUCT_T1n_MSW_MSB_ADDRESS_EEPROM	(0x000D) //diagramStruct.T1n
//#define DIAGRAM_STRUCT_T1n_MSW_LSB_ADDRESS_EEPROM	(0x000E) //diagramStruct.T1n
//#define DIAGRAM_STRUCT_T1n_LSW_MSB_ADDRESS_EEPROM	(0x000F) //diagramStruct.T1n
//#define DIAGRAM_STRUCT_T1n_LSW_LSB_ADDRESS_EEPROM	(0x0010) //diagramStruct.T1n
//
//#define DIAGRAM_STRUCT_Tn_MSW_MSB_ADDRESS_EEPROM	(0x0011) //diagramStruct.Tn
//#define DIAGRAM_STRUCT_Tn_MSW_LSB_ADDRESS_EEPROM	(0x0012) //diagramStruct.Tn
//#define DIAGRAM_STRUCT_Tn_LSW_MSB_ADDRESS_EEPROM	(0x0013) //diagramStruct.Tn
//#define DIAGRAM_STRUCT_Tn_LSW_LSB_ADDRESS_EEPROM	(0x0014) //diagramStruct.Tn
//
//#define DIAGRAM_STRUCT_n_ADDRESS_EEPROM				(0x0015) //diagramStruct.n
//
////фазы
//#define PHASE_MANIP_ADDRESS_EEPROM					(0x0020) //фазы


//время работы

#endif /* EEPROMDISTRTABLE_H_ */