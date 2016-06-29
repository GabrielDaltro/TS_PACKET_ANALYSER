/*****************************************************************
//TS_PACKET_ANALYSER.cpp
//Gabriel Daltro Duarte
//27/06/2016

******************************************************************/

#include "TS_PACKET_ANALYSER.h"
#include <iostream>
#include <cstdio>


 /***************************CLASSE TS_PACKET********************************************/

TS_PACKET::TS_PACKET (FILE*  ptrFileTS,  unsigned long int tsPacketNumber)
{
        TS_PACKET::setTS_PACKET(ptrFileTS,tsPacketNumber);
}

 void TS_PACKET::setTsPacketNumber (unsigned long int num)
 {
     tsPacketNum = num;
 }

 void TS_PACKET::setBytes (FILE*  ptrFileTS,  unsigned long int tsPacketNumber)
 {
     rewind(ptrFileTS);// Coloca o cursor no começo do arquivo por precaução
    fseek(ptrFileTS,188*(tsPacketNumber-1),SEEK_SET); // Coloca o cursor no inicio do pacote TS especificado pela variável num_ts_packet
    //Preenchimento do vetor bytes[]
    for (int i = 0; i < 188; i++)
    {
        bytes[i] = 0; // Zera a variável antes de prenche-la para evitar problemas com lixo
        fread(&(bytes[i]),1,1,ptrFileTS); // Lê um byte do arquivo e salva na posição correspondente do vetor Bytes[]
    }
 }

void TS_PACKET::setTS_PACKET(FILE* ptrFileTS,  unsigned long int tsPacketNumber)
{
    setTsPacketNumber(tsPacketNumber);
    setBytes (ptrFileTS,tsPacketNumber);

    header.setTS_PACKET_HEADER(bytes);
    ptrTABLE_PAT = NULL;
    ptrTABLE_PMT = NULL;
    if (header.getPID() == 0)
    {
        ptrTABLE_PAT = new TABLE_PAT;
        ptrTABLE_PAT->setTablePAT(bytes, header.getPayloadBegin());
        ptrTABLE_PMT = NULL;
    }
    else if (TABLE_PAT::getProgram_map_pid() == -1) // Se uma tabela map nunca foi inicializada
    {
        ptrTABLE_PAT = NULL;
        std::cout << std::endl << std::endl <<"Program_map_id desconhecido!" << std::endl << "Inicialize primeiro um PACKET com PAYLOAD PAT TABLE" << std::endl << std::endl;
    }
    else if (header.getPID() == TABLE_PAT::getProgram_map_pid()) // Se o pacote for uma tabela PMT
    {
        ptrTABLE_PAT = NULL;
        ptrTABLE_PMT = new TABLE_PMT;
        ptrTABLE_PMT->setTablePMT(bytes, header.getPayloadBegin());
    }
}

void TS_PACKET::printTSPacketBytes(void) const
{
    std::cout << std::endl << std::endl <<"TS PACKET " << tsPacketNum << std::endl << std::endl;
    for (int i = 0; i < 188; i++)
    {
        if (!(i%16)) // imprimi 16 números por linha
            std::cout << std::endl;
         printf("%0.2X  ", bytes[i]); // imprime o byte lido na tela
    }
}

 void TS_PACKET::printTSPacketHeader(void) const
 {
     header.printHeader();
 }


 /***************************CLASSE TS_PACKET_HEADER********************************************/

 TS_PACKET_HEADER::TS_PACKET_HEADER (){}

short int TS_PACKET_HEADER::getSyncByte() const
{
    return syncByte;
}

bool TS_PACKET_HEADER::getTransportErrorIndicator() const
{
    return transportErrorIndicator;
}

bool TS_PACKET_HEADER::getPayloadUnitStartPriority() const
{
    return payloadUnitStartPriority;
}

bool TS_PACKET_HEADER::getTransportPriority() const
{
    return transportPriority;
}

short int TS_PACKET_HEADER::getPID() const
{
    return PID;
}

short int TS_PACKET_HEADER::getTransportScramblingControl() const
{
    return transportScramblingControl;
}

short int TS_PACKET_HEADER::getAdaptationFieldControl() const
{
    return adaptationFieldControl;
}

short int TS_PACKET_HEADER::getContinuityCounter() const
{
    return continuityCounter;
}

short int TS_PACKET_HEADER::getAdpationFieldLenth() const
{
    return AdaptationFieldLength; // AQUI TINHA AdaptationFieldControl
}

short int TS_PACKET_HEADER::getPayloadBegin() const
{
    return payLoadBegin;
}

 void TS_PACKET_HEADER::setTS_PACKET_HEADER(const int* const ptrBytes)
{
    // Cada função set lê um determinado campo do cabeçalho e salva seu conteúdo no atributo correspodente
    setSyncByte (ptrBytes);
    setTransportErrorIndicator (ptrBytes);
    setPayloadUnitStartPriority (ptrBytes);
    setTransportPriority (ptrBytes);
    setPID (ptrBytes);
    setTransportScramblingControl (ptrBytes);
    setAdaptationFieldControl (ptrBytes);
    setContinuityCounter (ptrBytes);

    setAdpationFieldLength(ptrBytes);
    setPayloadBegin(ptrBytes);
}

void TS_PACKET_HEADER::setSyncByte (const int* const Vet)
{
    syncByte = Vet[0];
}

void TS_PACKET_HEADER::setTransportErrorIndicator (const int* const Vet)
{
    transportErrorIndicator =  (Vet[1]&0b10000000)>> 7 ;
}

void TS_PACKET_HEADER::setPayloadUnitStartPriority (const int* const Vet)
{
    payloadUnitStartPriority = (Vet[1]&0b01000000)>> 6;
}

void TS_PACKET_HEADER::setTransportPriority (const int* const Vet)
{
    transportPriority =  (Vet[1]&0b00100000)>> 5;
}

void TS_PACKET_HEADER::setPID (const int* const Vet)
{
    PID = ((Vet[1])&0b00011111)*256 + Vet[2];
}

void TS_PACKET_HEADER::setTransportScramblingControl (const int* const Vet)
{
    transportScramblingControl = (Vet[3]&0b11000000)>> 6 ;
}

void TS_PACKET_HEADER::setAdaptationFieldControl (const int* const Vet)
{
    adaptationFieldControl = (Vet[3]&0b00110000)>> 4 ;
}

void TS_PACKET_HEADER::setContinuityCounter (const int* const Vet)
{
    continuityCounter = (Vet[3])&0b00001111;
}

void TS_PACKET_HEADER::setAdpationFieldLength(const int* const Vet)
{
    AdaptationFieldLength = Vet[4]; // O comprimento do Aptation field é o 5º Byte do TS PACKET
}
void TS_PACKET_HEADER::setPayloadBegin (const int* const Vet)
{
    if (getAdaptationFieldControl() != 2)
    {
        setAdpationFieldLength (Vet);
        payLoadBegin = 5 + getAdpationFieldLenth();
    }
     else
        payLoadBegin = -1;

}

void TS_PACKET_HEADER::printHeader (void) const
{
    std::cout << std::endl << std::endl << "TS PACKET " << " HEADER ";
    std::cout << std::endl << std::endl <<  "Sync byte: " << std::hex << getSyncByte() << std::endl
              << "Transport error indicator: " <<  getTransportErrorIndicator() << std::endl
              << "Payload unit start indicator: " << getPayloadUnitStartPriority() << std::endl
              << "Transport priority: " << getTransportPriority() << std::endl
              << "PID: " << std::dec << getPID() << std::endl
              << "Transport scrambling control: " << getTransportScramblingControl() << std::endl
              << "Adaptation field control: " << getAdaptationFieldControl() << std::endl
              << "Continuity counter: " << getContinuityCounter() << std::endl;
}


/********************************CLASSE TABLE_PAT******************************************/

//Inicialização de variável static
int TABLE_PAT::program_map_pid = -1;

short int TABLE_PAT::getTable_ID (void) const
{
    return table_id;
}
bool TABLE_PAT::getSection_syntax_indicator (void) const
{
    return section_syntax_indicator;
}
int TABLE_PAT::getSection_length (void) const
{
    return section_length;
}
int TABLE_PAT::getTransport_stream_id (void) const
{
    return transport_stream_id;
}
short int TABLE_PAT::getVersion_number (void) const
{
    return version_number;
}
bool TABLE_PAT::getCurrent_next_indicator (void) const
{
    return current_next_indicator;
}
short int TABLE_PAT::getSection_number (void) const
{
    return section_number;
}
short int TABLE_PAT::getLast_section_number (void) const
{
    return last_section_number;
}
int TABLE_PAT::getProgram_number (void) const
{
    return program_number;
}
int TABLE_PAT::getNetwork_PID (void) const
{
    return network_PID;
}
int TABLE_PAT::getProgram_map_pid (void)
{
    return program_map_pid;
}
long int TABLE_PAT::getCRC (void) const
{
    return CRC;
}

void TABLE_PAT::setTablePAT (const int* const ptrBytes, short int payloadBegin)
{
      setTable_ID(ptrBytes, payloadBegin );
      setSection_syntax_indicator (ptrBytes, payloadBegin );
      setSection_length (ptrBytes, payloadBegin );
      setTransport_stream_id (ptrBytes, payloadBegin );
      setVersion_number (ptrBytes, payloadBegin );
      setCurrent_next_indicator (ptrBytes, payloadBegin );
      setSection_number (ptrBytes, payloadBegin );
      setLast_section_number (ptrBytes, payloadBegin );
      setProgram_number (ptrBytes, payloadBegin );
      setNetwork_PID (ptrBytes, payloadBegin );
      setProgram_map_pid (ptrBytes, payloadBegin );
      setCRC (ptrBytes, payloadBegin );
}

void TABLE_PAT::setTable_ID (const int* const ptrBytes, short int payloadBegin)
{
    table_id = ptrBytes[payloadBegin];
}

void TABLE_PAT::setSection_syntax_indicator(const int* const ptrBytes, short int payloadBegin)
{
    section_syntax_indicator = (ptrBytes[payloadBegin+1]&0b10000000)>>7;
}

void TABLE_PAT::setSection_length (const int* const ptrBytes, short int payloadBegin)
{
    section_length = ((ptrBytes[payloadBegin+1])&0b00001111)*256 + ptrBytes[payloadBegin+2];
}
void TABLE_PAT::setTransport_stream_id (const int* const ptrBytes, short int payloadBegin)
{
    transport_stream_id = ptrBytes[payloadBegin+3]*256 + ptrBytes[payloadBegin+4];
}
void TABLE_PAT::setVersion_number (const int* const ptrBytes, short int payloadBegin)
{
    version_number = (ptrBytes[payloadBegin+5]&0b00111110)>>1;
}
void TABLE_PAT::setCurrent_next_indicator (const int* const ptrBytes, short int payloadBegin)
{
    current_next_indicator = (ptrBytes[payloadBegin+5]&0b00000001);
}
void TABLE_PAT::setSection_number (const int* const ptrBytes, short int payloadBegin)
{
    section_number = ptrBytes[payloadBegin+6];
}
void TABLE_PAT::setLast_section_number (const int* const ptrBytes, short int payloadBegin)
{
    last_section_number = ptrBytes[payloadBegin+7];
}


void TABLE_PAT::setProgram_number (const int* const ptrBytes, short int payloadBegin)
{
    program_number = ptrBytes[payloadBegin+8]*256 + ptrBytes[payloadBegin+9];
}
void TABLE_PAT::setNetwork_PID (const int* const ptrBytes, short int payloadBegin)
{
    if (getProgram_number() == 0) // se program network pid existe
        network_PID = (ptrBytes[payloadBegin+10]&0b00011111)*256 + ptrBytes[payloadBegin+11];
    else
        network_PID = -1; // NÃO EXISTE
}
void TABLE_PAT::setProgram_map_pid (const int* const ptrBytes, short int payloadBegin)
{
    if (getProgram_number() != 0) // Se program_map_pid existe
        program_map_pid = (ptrBytes[payloadBegin+10]&0b00011111)*256 + ptrBytes[payloadBegin+11];
    else
        program_map_pid = -1; // NÃO EXISTE

}

void TABLE_PAT::setCRC (const int* const ptrBytes, short int payloadBegin)
{
     int crcBegin;
    crcBegin = payloadBegin + 3 + getSection_length() - 4;
    CRC = ptrBytes[crcBegin]*16777216 + ptrBytes[crcBegin+1]*65536 + ptrBytes[crcBegin+2]*256 + ptrBytes[crcBegin+3];
}

void TABLE_PAT::printTABLE_PAT(void) const
{
    std::cout << std::endl << std::endl << "PAT TABLE " << std::endl
              << "table_id: " << getTable_ID() << std::endl
              << "section_syntax_indicator: " << getSection_syntax_indicator() << std::endl
              << "section length: " << getSection_length() << std::endl
              << "transport_stream_id: " << getTransport_stream_id() << std::endl
              << "version_number: " << getVersion_number() << std::endl
              << "current_next_indicator: " << getCurrent_next_indicator() << std::endl
              << "section_number: " << getSection_number() << std::endl
              << "last_section_number: " << getSection_number() << std::endl
              << "program_number: " << getProgram_number() << std::endl;
    if (getProgram_number() == 0)
        std::cout << "network_PID: " << getNetwork_PID() << std::endl;
    else
        std::cout << "program_map_PID: " << getProgram_map_pid() << std::endl;

    printf("CRC_32: %#X\n", getCRC() );
}

/********************************CLASSE TABLE_PMT******************************************/

short int TABLE_PMT::getTable_id (void) const
{
    return table_id;
}
bool TABLE_PMT::getSection_syntax_indicator (void) const
{
    return section_syntax_indicator;
}
int TABLE_PMT::getSection_length (void) const
{
    return section_length;
}
int TABLE_PMT::getProgram_number (void) const
{
    return program_number;
}
short int TABLE_PMT::getVersion_number (void) const
{
    return version_number;
}
bool TABLE_PMT::getCurrent_next_indicator (void) const
{
    return current_next_indicator;
}
short int TABLE_PMT::getSection_number (void) const
{
    return section_number;
}
short int TABLE_PMT::getLast_section_number (void) const
{
    return last_section_number;
}
int TABLE_PMT::getPCR_PID (void) const
{
    return PCR_PID;
}
int TABLE_PMT::getProgram_info_length (void) const
{
    return program_info_length;
}

int* TABLE_PMT::getProgram_info_descriptor(void) const
{
    return program_info_descriptor;
}


short int TABLE_PMT::getStream_type (void) const
{
    return stream_type;
}
int TABLE_PMT::getElementary_PID (void) const
{
    return elementary_PID;
}
int TABLE_PMT::getES_info_length (void) const
{
    return ES_info_length;
}

long int TABLE_PMT::getCRC_32 (void) const
{
    return CRC_32;
}


void TABLE_PMT::setTablePMT (const int* const ptrBytes, short int payloadBegin)
{
    setTable_id (ptrBytes,payloadBegin);
    setSection_syntax_indicator (ptrBytes,payloadBegin);
    setSection_length (ptrBytes,payloadBegin);
    setProgram_number (ptrBytes,payloadBegin);
    setVersion_number (ptrBytes,payloadBegin);
    setCurrent_next_indicator (ptrBytes,payloadBegin);
    setSection_number (ptrBytes,payloadBegin);
    setLast_section_number (ptrBytes,payloadBegin);
    setPCR_PID (ptrBytes,payloadBegin);
    setProgram_info_length (ptrBytes,payloadBegin);
    setProgram_info_descriptor(ptrBytes,payloadBegin);
    setStream_type (ptrBytes,payloadBegin);
    setElementary_PID (ptrBytes,payloadBegin);
    setES_info_length (ptrBytes,payloadBegin);
    setCRC_32 (ptrBytes,payloadBegin);
}

void TABLE_PMT::setTable_id (const int* const ptrBytes, short int payloadBegin)
{
    table_id = ptrBytes[payloadBegin];
}
void TABLE_PMT::setSection_syntax_indicator (const int* const ptrBytes, short int payloadBegin)
{
    section_syntax_indicator = (ptrBytes[payloadBegin+1]&0b10000000)>>7;
}
void TABLE_PMT::setSection_length (const int* const ptrBytes, short int payloadBegin)
{
    section_length = (ptrBytes[payloadBegin+1]&0b00001111)*256 + ptrBytes[payloadBegin+2];
}
void TABLE_PMT::setProgram_number (const int* const ptrBytes, short int payloadBegin)
{
    program_number = ptrBytes[payloadBegin+3]*256 + ptrBytes[payloadBegin+4];
}
void TABLE_PMT::setVersion_number (const int* const ptrBytes, short int payloadBegin)
{
    version_number = (ptrBytes[payloadBegin+5]&0b00111110)>>1;
}
void TABLE_PMT::setCurrent_next_indicator (const int* const ptrBytes, short int payloadBegin)
{
    current_next_indicator = ptrBytes[payloadBegin+5]&0b00000001;
}
void TABLE_PMT::setSection_number (const int* const ptrBytes, short int payloadBegin)
{
    section_number = ptrBytes[payloadBegin + 6];
}
void TABLE_PMT::setLast_section_number (const int* const ptrBytes, short int payloadBegin)
{
    last_section_number = ptrBytes[payloadBegin + 7];
}
void TABLE_PMT::setPCR_PID (const int* const ptrBytes, short int payloadBegin)
{
    PCR_PID = (ptrBytes[payloadBegin+8]&0b00011111)*256 + ptrBytes[payloadBegin + 9];
}
void TABLE_PMT::setProgram_info_length (const int* const ptrBytes, short int payloadBegin)
{
    program_info_length = (ptrBytes[payloadBegin + 10]&0b00001111)*256 + ptrBytes[payloadBegin+11];
}

void TABLE_PMT::setProgram_info_descriptor(const int* const ptrBytes, short int payloadBegin)
{
    int i;
    int descriptorBegin = payloadBegin + 13;
    int length = getProgram_info_length();
    if (length != 0)
    {
        program_info_descriptor = new int[length];
        for (int i = descriptorBegin; i <= length; i++)
            program_info_descriptor[i] = ptrBytes[i];
    }
    else
        program_info_descriptor = NULL;
}


void TABLE_PMT::setStream_type (const int* const ptrBytes, short int payloadBegin)
{
    stream_type = ptrBytes[payloadBegin+12];
}
void TABLE_PMT::setElementary_PID (const int* const ptrBytes, short int payloadBegin)
{
    elementary_PID = (ptrBytes[payloadBegin+13]&0b00011111)*256 + ptrBytes[payloadBegin+14];
}
void TABLE_PMT::setES_info_length (const int* const ptrBytes, short int payloadBegin)
{
    ES_info_length = (ptrBytes[payloadBegin+15]&0b00001111)*256 + ptrBytes[payloadBegin+16];
}


void TABLE_PMT::setCRC_32 (const int* const ptrBytes, short int payloadBegin)
{
    int crcBegin;
    crcBegin = payloadBegin + 3 + getSection_length() - 4;
    CRC_32 = ptrBytes[crcBegin]*16777216 + ptrBytes[crcBegin+1]*65536 + ptrBytes[crcBegin+2]*256 + ptrBytes[crcBegin+3];
}

void TABLE_PMT::printTABLE_PMT(void) const
{
    int* ptr = getProgram_info_descriptor();
    std::cout << std::endl<< std::endl << "PMT TABLE" << std::endl
              <<"table_id: " << getTable_id() << std::endl
              <<"section_syntax_indicator: " << getSection_syntax_indicator() << std::endl
              <<"section_length: " << getSection_length() << std::endl
              <<"program_number: " << getProgram_number()<< std::endl
              <<"version_number: " << getVersion_number() << std::endl
              <<"current_next_indicator: " << getCurrent_next_indicator()<< std::endl
              <<"section_number: " << getSection_number()<< std::endl
              <<"last_section_number: " << getLast_section_number()<< std::endl
              <<"PCR_PID: " << getPCR_PID() << std::endl
              <<"program_info_length: " << getProgram_info_length() << std::endl;
   if (getProgram_info_descriptor() != NULL)
    {
        std::cout << "  descriptor: ";
        for (int i = 0; i < getProgram_info_length(); i++ )
            std::cout << ptr[i] << " ";
    }
    else
        std::cout << "  descriptor: SEM CONTEUDO" << std::endl;

    std::cout <<"stream_type: " << getStream_type() << std::endl
              <<"elementary_PID: " << getElementary_PID() << std::endl
              <<"ES_info_length: " << getES_info_length()<< std::endl;
    printf("CRC_32: %#X\n",getCRC_32());
}

