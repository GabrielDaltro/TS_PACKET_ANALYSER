/*****************************************************************
//TS_PACKET_ANALYSER.h
//Gabriel Daltro Duarte
//27/06/2016

******************************************************************/
#include <cstdlib>
#include <cstdio>

typedef struct {
    int stream_type;
    int elementary_PID;
    int ES_info_length;
    int* program_info_descriptor;
} program_info;

class TABLE_PAT {

    public:
        //TABLE_PAT (const int* const ptrBytes);

        void setTablePAT (const int* const ptrBytes, short int payloadBegin);

        short int getTable_ID (void) const;
        bool getSection_syntax_indicator (void) const;
        int getSection_length (void) const;
        int getTransport_stream_id (void) const;
        short int getVersion_number (void) const;
        bool getCurrent_next_indicator (void) const;
        short int getSection_number (void) const;
        short int getLast_section_number (void) const;
        int getProgram_number (void) const;
        int getNetwork_PID (void) const;
        static int getProgram_map_pid (void);
        long int getCRC (void) const;

        void printTABLE_PAT(void) const;

    private:

        int table_id;
        bool section_syntax_indicator;
        int section_length;
        int transport_stream_id;
        short int version_number;
        bool current_next_indicator;
        short int section_number;
        short int last_section_number;
        int program_number;
        int network_PID;
        static int program_map_pid;
        long int CRC;

        void setTable_ID (const int* const ptrBytes, short int payloadBegin);
        void setSection_syntax_indicator(const int* const ptrBytes, short int payloadBegin);
        void setSection_length (const int* const ptrBytes, short int payloadBegin);
        void setTransport_stream_id (const int* const ptrBytes, short int payloadBegin);
        void setVersion_number (const int* const ptrBytes, short int payloadBegin);
        void setCurrent_next_indicator (const int* const ptrBytes, short int payloadBegin);
        void setSection_number (const int* const ptrBytes, short int payloadBegin);
        void setLast_section_number (const int* const ptrBytes, short int payloadBegin);
        void setProgram_number (const int* const ptrBytes, short int payloadBegin);
        void setNetwork_PID (const int* const ptrBytes, short int payloadBegin);
        void setProgram_map_pid (const int* const ptrBytes, short int payloadBegin);
        void setCRC (const int* const ptrBytes, short int payloadBegin);

};

class TABLE_PMT {

    public:

        void setTablePMT (const int* const ptrBytes, short int payloadBegin);

        short int getTable_id (void) const;
        bool getSection_syntax_indicator (void) const;
        int getSection_length (void) const;
        int getProgram_number (void) const;
        short int getVersion_number (void) const;
        bool getCurrent_next_indicator (void) const;
        short int getSection_number (void) const;
        short int getLast_section_number (void) const;
        int getPCR_PID (void) const;
        int getProgram_info_length (void) const;
        short int getStream_type (void) const;
        int getElementary_PID (void) const;
        int getES_info_length (void) const;
        long int getCRC_32 (void) const;

        int* getProgram_info_descriptor(void) const;

        void printTABLE_PMT(void) const;

    private:

        short int table_id;
        bool section_syntax_indicator;
        int section_length;
        int program_number;
        short int version_number;
        bool current_next_indicator;
        short int section_number;
        short int last_section_number;
        int PCR_PID;
        int program_info_length;
        int* program_info_descriptor;
        short int stream_type;
        int elementary_PID;
        int ES_info_length;
        long int CRC_32;

        void setTable_id (const int* const ptrBytes, short int payloadBegin);
        void setSection_syntax_indicator (const int* const ptrBytes, short int payloadBegin);
        void setSection_length (const int* const ptrBytes, short int payloadBegin);
        void setProgram_number (const int* const ptrBytes, short int payloadBegin);
        void setVersion_number (const int* const ptrBytes, short int payloadBegin);
        void setCurrent_next_indicator (const int* const ptrBytes, short int payloadBegin);
        void setSection_number (const int* const ptrBytes, short int payloadBegin);
        void setLast_section_number (const int* const ptrBytes, short int payloadBegin);
        void setPCR_PID (const int* const ptrBytes, short int payloadBegin);
        void setProgram_info_length (const int* const ptrBytes, short int payloadBegin);

        void setProgram_info_descriptor(const int* const ptrBytes, short int payloadBegin);

        void setStream_type (const int* const ptrBytes, short int payloadBegin);
        void setElementary_PID (const int* const ptrBytes, short int payloadBegin);
        void setES_info_length (const int* const ptrBytes, short int payloadBegin);

        void setCRC_32 (const int* const ptrBytes, short int payloadBegin);
};

class TS_PACKET_HEADER{
public:
        // Construtor da classe
        TS_PACKET_HEADER ();

        // Com esta função é possível alterar o conteudo de um objeto TS_PACKET
        void setTS_PACKET_HEADER(const int* const ptrBytes);
        //Parâmetros:
        //ptrFileTS: Ponteiro para o arquivo TS
        //tsPacketNumber: Posição do pacote no arquivo TS

        //void setTsPacketNum (unsigned long int num);

        // Funções get das variáveis do HEADER
        short int getSyncByte(void) const;
        bool getTransportErrorIndicator(void) const;
        bool getPayloadUnitStartPriority(void)const;
        bool getTransportPriority(void) const;
        short int getPID(void) const;
        short int getTransportScramblingControl(void) const;
        short int getAdaptationFieldControl(void) const;
        short int getContinuityCounter(void) const;

        short int getAdpationFieldLenth (void) const;
        short int getPayloadBegin (void) const;

        // Imprime os conteúdos dos campos do cabeçalho do pacote TS
        void printHeader (void) const;

    private:
        //unsigned long int tsPacketNum; // Variável que armazena a posição do pacote TS  no arquivo

        //funções set dos atributos do HEADER
        void setSyncByte (const int* const Vet);
        void setTransportErrorIndicator (const int* const Vet);
        void setPayloadUnitStartPriority (const int* const Vet);
        void setTransportPriority (const int* const Vet);
        void setPID (const int* const Vet);
        void setTransportScramblingControl (const int* const Vet);
        void setAdaptationFieldControl (const int* const Vet);
        void setContinuityCounter (const int* const Vet);

        void setAdpationFieldLength (const int* const Vet);
        void setPayloadBegin (const int* const Vet);

        //variáveis do HEADER
        short int syncByte;
        bool transportErrorIndicator;
        bool payloadUnitStartPriority;
        bool transportPriority;
        short int PID;
        short int transportScramblingControl;
        short int adaptationFieldControl;
        short int continuityCounter;

        //
        short int payLoadBegin;
        short int AdaptationFieldLength;
};

class TS_PACKET {
    public:

        TS_PACKET_HEADER header;
        TABLE_PAT* ptrTABLE_PAT;
        TABLE_PMT* ptrTABLE_PMT;

        // Construtor da classe
        TS_PACKET (FILE* ptrFileTS,  unsigned long int tsPacketNumber );
        //Parâmetros do construtor:
        //ptrFileTS: Ponteiro para o arquivo TS
        //tsPacketNumber: Posição do pacote no arquivo TS

        // Com esta função é possível alterar o conteudo de um objeto TS_PACKET
        void setTS_PACKET(FILE* ptrFileTS,  unsigned long int tsPacketNumber);
        //Parâmetros:
        //ptrFileTS: Ponteiro para o arquivo TS
        //tsPacketNumber: Posição do pacote no arquivo TS

        // Imprime o valor dos 188 bytes que formam o pacote TS
        void printTSPacketBytes(void)  const;

        // Imprime o cabeçalho do pacote
        void printTSPacketHeader(void) const;

    private:
        void setBytes (FILE*  ptrFileTS,  unsigned long int tsPacketNumber);
        void setTsPacketNumber (unsigned long int num);
        int bytes[188]; // Armazena os 188 bytes do pacote lido do arquivo
        unsigned long int tsPacketNum; // Variável que armazena a posição do pacote TS  no arquivo




};




