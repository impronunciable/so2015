#include "srv.h"

/*
    Nuevo y mejorado servidor.
    No admite ingreso de nuevos pares cliente-servidor
*/
void servidor(int mi_cliente)
{
    MPI_Status status; int origen, tag;
    int hay_pedido_local = false;
    int listo_para_salir = false;

    int N = cant_ranks/2;       // Para simplificar
    int seq_num;                // Número de secuencia obtenido en un request
    int highest_seq_num = 0;    // Mayor número de secuencia visto

    bool vivo[N];               // Indica cuáles nodos están vivos
    int cant_vivos = N;         // La cantidad de vivos
    bool reply_deferred[N];     // A cuales tengo que mandar si me pidieron

    int expected_replies = N-1; // Respuestas esperadas
    bool respondio[N];          // Indica si i respondió

    for (int i = 0; i < N; ++i){
        reply_deferred[i] = false;
        vivo[i] = true;             //todos están vivos al principio
        respondio[i] = false;
    }

    while( ! listo_para_salir ) {
        int buffer;

        // Recibo mensajes. Si me llega un seq_num, lo guardo en buffer
        MPI_Recv(&buffer, 1, MPI_INT, ANY_SOURCE, ANY_TAG, COMM_WORLD, &status);
        origen = status.MPI_SOURCE;
        tag = status.MPI_TAG;

        // chequeo tags y actúo en consecuencia
        switch(tag){
        case TAG_PEDIDO: {
            assert(origen == mi_cliente);
            debug("Mi cliente solicita acceso exclusivo");

            assert(hay_pedido_local == false);
            hay_pedido_local = true;

            // reseteo el array de los que respondieron
            for (int i = 0; i < N; ++i){
                respondio[i] = false;
            }

            // La cantidad de respuestas esperadas es la cantidad de vivos
            // menos el mismo [no espero una respuesta de mí mismo]
            expected_replies = cant_vivos-1;

            seq_num = highest_seq_num + 1;
            // Envío a todos un REQUEST
            for (int i = 0; i < N; ++i){
                if (vivo[i] && i != mi_nro){
                    MPI_Send(&seq_num, 1, MPI_INT, i*2, TAG_REQUEST, COMM_WORLD);
                }
            }

            // Si soy el último, me doy permiso a mí mismo
            if (cant_vivos == 1){
                debug("Dándole permiso");
                MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, COMM_WORLD);
            }
            break;
        }

        case TAG_LIBERO:{
            assert(origen == mi_cliente);
            debug("Mi cliente libera su acceso exclusivo");

            assert(hay_pedido_local == true);
            hay_pedido_local = false;

            // Cuando libero, si me quedaba gente que me pedía acceso exclusivo,
            // les indico que pueden tenerlo, porque ya terminé con eso
            for (int i = 0; i < N; ++i){
                if (vivo[i] && reply_deferred[i]){
                    reply_deferred[i] = false;
                    MPI_Send(NULL, 0, MPI_INT, i*2, TAG_REPLY, COMM_WORLD);
                }
            }
            break;
        }
        case TAG_REQUEST:{
            int k = buffer; // seq_num recibido del emisor
            bool defer;
            // Si el número que recibí resulta mayor que el más alto, entonces
            // debe ser el más alto
            highest_seq_num = max(highest_seq_num, k);
            defer = hay_pedido_local && ((k > seq_num) || (k == seq_num && origen/2 > mi_nro) );

            if (defer){
                // Luego voy a tener que mandar un reply,
                // pero AHORA me toca a mí
                reply_deferred[origen/2] = true;
            } else {
                // Le indico al que me mandó que puede tener acceso exclusivo
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
            // Estoy listo para salir :)
            listo_para_salir = true;
            break;
        }
        case TAG_SE_VA_UNO:{
            // Marcamos como muerto al emisor
            vivo[origen/2] = false;
            // ...y actualizamos la cantidad de vivos
            cant_vivos--;

            if (respondio[origen/2] || !hay_pedido_local){
                // Si ya respondió, o no hay ningún pedido local, termino
                break;
            }
            // si no respondió, continúo como si fuera TAG_REPLY
            // o sea, se ejecuta lo que está acá abajo.
            // Notar falta de break
        }
        case TAG_REPLY:{
            // Seteo que el que mandó el mensaje respondió
            respondio[origen/2] = true;
            expected_replies--;

            // todos respondieron => consigo acceso exclusivo
            if (expected_replies == 0){
                debug("Dándole permiso");
                MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, COMM_WORLD);
            }
            break;
        }
        default:
            // Esto no se supone que pase
            debug("Alguien mandó basura :(");
        break;

        }
    }
}
