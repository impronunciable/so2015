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

    bool vivo[N];
    int cant_vivos = N;
    bool reply_deferred[N]; // NO ADMITE INGRESO DE NODOS
    //int expected_replies;
    bool respondio[N];
    for (int i = 0; i < N; ++i){
        reply_deferred[i] = false;
        vivo[i] = true;
        respondio[i] = false;
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

            assert(hay_pedido_local == false); //meh
            hay_pedido_local = true;

            for (int i = 0; i < N; ++i){
                respondio[i] = false;
            }

            seq_num = highest_seq_num + 1;
            // Envío a todos un REQUEST
            for (int i = 0; i < N; ++i){
                if (vivo[i] && i != mi_nro){
                    MPI_Send(&seq_num, 1, MPI_INT, i*2, TAG_REQUEST, COMM_WORLD);
                }
            }

            // Soy el último
            if (cant_vivos == 1){
                debug("Dándole permiso");
                MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, COMM_WORLD);
            }
            break;
        }

        case TAG_LIBERO:{
            assert(origen == mi_cliente);
            debug("Mi cliente libera su acceso exclusivo");

            assert(hay_pedido_local == TRUE);
            hay_pedido_local = FALSE;

            for (int i = 0; i < N; ++i){
                if (vivo[i] && reply_deferred[i]){
                    reply_deferred[i] = false;
                    MPI_Send(NULL, 0, MPI_INT, i*2, TAG_REPLY, COMM_WORLD);
                }
            }
            break;
        }
        case TAG_REQUEST:{
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
            // Mando mensajes a todos, indicando que me fui
            for (int i = 0; i < N; ++i){
                if (vivo[i] && i != mi_nro){
                    MPI_Send(NULL, 0, MPI_INT, i*2, TAG_SE_VA_UNO, COMM_WORLD);
                }
            }
            listo_para_salir = TRUE;
            break;
        }
        case TAG_SE_VA_UNO:{
            // Se muere el origen
            vivo[origen/2] = false;
            cant_vivos--;
            if (respondio[origen/2] || !hay_pedido_local){
                break;
            }
            // si no respondió, continúo como si fuera TAG_REPLY
            // o sea, se ejecuta lo que está acá abajo.
            // Notar falta de break
        }
        case TAG_REPLY:{
            respondio[origen/2] = true;

            int i;
            for(i = 0; i < N; ++i){
                if (vivo[i] && i != mi_nro && !respondio[i]){
                    break;
                }
            }

            // todos responden
            if (i == N){
                debug("Dándole permiso");
                MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, COMM_WORLD);
            }
            break;
        }
        default:
            debug("Alguien mandó basura :(");
        break;

        }
    }
}
