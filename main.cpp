/*****************************************************************
//main.c
//Gabriel Daltro Duarte
//27/06/2016

******************************************************************/

#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "TS_PACKET_ANALYSER.h"

int main()
{
   unsigned long int pacoteNum = 1;
   FILE* ptrArquivoTS;


   std::cout << "****************************TS PACKET ANALYSER********************************** " << std::endl;

   ptrArquivoTS = fopen("video.ts", "rb");
   if (ptrArquivoTS == NULL)
    {
       std::cout << "ERRO AO ABRIR O ARQUIVO!" << std::endl << std::endl;
       return 0;
    }
   else
        std::cout << "ARQUIVO ABERTO COM SUCESSO!" << std::endl << std::endl;

    TS_PACKET tsPacketOne(ptrArquivoTS,1); // Declara um packet cujo payload é uma tabela PAT - Assim o valor de program_map_`PID passa a ser conhecido

    std::cout << std::endl << std::endl <<"TUTORIAL" << std::endl << std::endl
              << "DIGITE O NUMERO DA SEQUENCIA DO PACOTE NO ARQUIVO" << std::endl
              << "SERA EXIBITO SEU HEADER" << std::endl
              << "SE O PAYLOAD FOR UMA PAT TABLE, SERA EXIBIDO AS PRINCIPAIS INFORMACOES" << std::endl
              << "SE O PAYLOAD FOR UMA PMT TABLE, SERA EXIBIDO AS PRINCIPAIS INFORMACOES" << std::endl
              << "SE O TS PACKET FOR VAZIO, SERA EXIBIDO NULL PACKET"  << std::endl;
    while (1)
    {
        std::cout << std::endl << "Digite o numero do pacote a ser analisado: " ;
        std::cin >> pacoteNum;

        tsPacketOne.setTS_PACKET (ptrArquivoTS,pacoteNum);
        tsPacketOne.printTSPacketBytes();
        tsPacketOne.printTSPacketHeader();

        if (tsPacketOne.ptrTABLE_PAT != NULL)
        {
            tsPacketOne.ptrTABLE_PAT->printTABLE_PAT();
        }
        else if (tsPacketOne.ptrTABLE_PMT != NULL )
            tsPacketOne.ptrTABLE_PMT->printTABLE_PMT();
        else if (tsPacketOne.header.getPID() == 8191)
            std::cout << std::endl << std::endl << "NULL PACKET" << std::endl;

        getchar();
        getchar();
        system("cls");
    }

   fclose (ptrArquivoTS);
   return 0;
}
