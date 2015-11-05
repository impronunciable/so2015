#include "srv.h"

/*
    Nuevo y mejorado servidor.
    No admite ingreso de nuevos pares cliente-servidor
    WIP
*/
void servidor(int mi_cliente)
{
    MPI_Status status; int origen, tag;
    int hay_pedido_local = false;
    int listo_para_salir = false;

    int N = cant_ranks/2;
    int seq_num;
    int highest_seq_num = 0;

    bool reply_deferred[N]; // NO ADMITE INGRESO DE NODOS
    for (int i = 0; i < N; ++i){
        reply_deferred[i] = false;
    }

    while( ! listo_para_salir ) {
        int buffer;

        MPI_Recv(&buffer, 1, MPI_INT, ANY_SOURCE, ANY_TAG, COMM_WORLD, &status);
        origen = status.MPI_SOURCE;
        tag = status.MPI_TAG;

        switch(tag){
        case TAG_PEDIDO: {
            assert(origen == mi_cliente);
            debug("Mi cliente solicita acceso exclusivo");

            assert(hay_pedido_local == FALSE); //meh
            hay_pedido_local = true;

            seq_num = highest_seq_num + 1;
            // Envío a todos un REQUEST
            for (int i = 0; i < N; ++i){
                if (i != mi_nro){
                    MPI_Send(&seq_num, 1, MPI_INT, i*2, TAG_REQUEST, COMM_WORLD);
                }
            }

            // Espero a que todos me digan que SÍ
            for(int i = 0; i < N; ++i){
                if (i != mi_nro){
                    MPI_Recv(NULL, 0, MPI_INT, i*2, TAG_REPLY, COMM_WORLD, &status);
                }
            }

            debug("Dándole permiso");
            MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, COMM_WORLD);
            break;
        }
        case TAG_LIBERO:{
            assert(origen == mi_cliente);
            debug("Mi cliente libera su acceso exclusivo");

            assert(hay_pedido_local == TRUE);
            hay_pedido_local = FALSE;

            for (int i = 0; i < N; ++i){
                if (reply_deferred[i]){
                    reply_deferred[i] = false;
                    MPI_Send(NULL, 0, MPI_INT, i*2, TAG_REPLY, COMM_WORLD);
                }
            }

            break;
        }
        case TAG_REQUEST:{
            //FIXME: k NO EXISTE! Crearlo y obtenerlo de Recv!
            int k = buffer;
            bool defer;
            highest_seq_num = max(highest_seq_num, k);
            defer = hay_pedido_local && ((k > seq_num) || (k == seq_num && origen/2 > mi_nro) );

            if (defer){
                reply_deferred[origen/2] = true;
            } else {
                MPI_Send(NULL, 0, MPI_INT, origen, TAG_REPLY, COMM_WORLD);
            }
            break;
        }
        case TAG_TERMINE:{
            assert(origen == mi_cliente);
            debug("Mi cliente avisa que terminó");
            //TODO: Arreglar esto. Es decir, indicar que ya no estoy
            //MPI_Send(NULL, 0, MPI_INT, origen, TAG_REPLY, COMM_WORLD);
            listo_para_salir = TRUE;
            break;
        }
        case TAG_REPLY:{
            debug("Esto no debería pasar...");
            break;
        }
        default:
            debug("Alguien mandó basura :(");
        break;

        }


    }

}
