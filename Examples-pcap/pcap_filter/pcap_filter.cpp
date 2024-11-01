#define HAVE_REMOTE
#include <pcap.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iomanip>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <cctype>

#pragma comment(lib, "ws2_32.lib")

// Estructura del encabezado IP (IPv4)
struct ip_header {
    unsigned char ip_header_len : 4;
    unsigned char ip_version : 4;
    unsigned char ip_tos;
    unsigned short ip_total_length;
    unsigned short ip_id;
    unsigned short ip_frag_offset;
    unsigned char ip_ttl;
    unsigned char ip_protocol;
    unsigned short ip_checksum;
    struct in_addr ip_src, ip_dst;
};

// Estructura para almacenar datos del paquete
struct PacketData {
    std::string ip_origen;
    int puerto_origen;
    std::string ip_destino;
    int puerto_destino;
    std::string protocolo;
    int longitud;
    std::string data;
};

// Función para extraer datos en formato string
std::string getDataAsString(const u_char* data, int length) {
    std::string dataStr;
    for (int i = 0; i < length && i < 40; i++) { // Limita a 40 caracteres
        if (isprint(data[i])) {
            dataStr += data[i];
        }
        else {
            dataStr += '.';
        }
    }
    if (length > 40) {
        dataStr += "...";  // Indica que se truncó
    }
    return dataStr;
}

// Función para imprimir el encabezado solo una vez
void printHeader() {
    std::cout << "\033[2J\033[1;1H";  // Código ANSI para limpiar pantalla y mover el cursor al inicio
    std::cout << std::left << std::setw(18) << "IP Origen"
        << std::setw(14) << "Puerto Origen"
        << std::setw(16) << "Puerto Destino"
        << std::setw(18) << "IP Destino"
        << std::setw(10) << "Protocolo"
        << std::setw(10) << "Longitud"
        << std::setw(40) << "Datos" << std::endl;
    std::cout << std::string(128, '-') << std::endl;
}

// Manejador de paquetes capturados
void packetHandler(u_char* userData, const struct pcap_pkthdr* packetHeader, const u_char* packetData) {
    auto& buffer = *reinterpret_cast<std::vector<PacketData>*>(userData);

    const int ethernetHeaderLen = 14;
    struct ip_header* ipHeader = (struct ip_header*)(packetData + ethernetHeaderLen);
    if (ipHeader->ip_version != 4) {
        return;  // Salta paquetes que no son IPv4
    }

    int puerto_origen = 0;
    int puerto_destino = 0;
    std::string protocolo;

    if (ipHeader->ip_protocol == IPPROTO_TCP || ipHeader->ip_protocol == IPPROTO_UDP) {
        struct transport_header {
            unsigned short src_port;
            unsigned short dst_port;
        };
        transport_header* th = (transport_header*)(packetData + ethernetHeaderLen + (ipHeader->ip_header_len * 4));

        puerto_origen = ntohs(th->src_port);
        puerto_destino = ntohs(th->dst_port);

        protocolo = (ipHeader->ip_protocol == IPPROTO_TCP) ? "TCP" : "UDP";
    }
    else if (ipHeader->ip_protocol == IPPROTO_ICMP) {
        protocolo = "ICMP";
    }
    else {
        protocolo = "Otro";
    }

    int dataOffset = ethernetHeaderLen + (ipHeader->ip_header_len * 4);
    std::string data = getDataAsString(packetData + dataOffset, packetHeader->len - dataOffset);

    buffer.push_back(PacketData{
        inet_ntoa(ipHeader->ip_src),
        puerto_origen,
        inet_ntoa(ipHeader->ip_dst),
        puerto_destino,
        protocolo,
        (int)packetHeader->len,
        data
        });

    if (buffer.size() == 5) {
        printHeader();

        for (const auto& packet : buffer) {
            std::cout << std::left << std::setw(18) << packet.ip_origen
                << std::setw(14) << packet.puerto_origen
                << std::setw(16) << packet.puerto_destino
                << std::setw(18) << packet.ip_destino
                << std::setw(10) << packet.protocolo
                << std::setw(10) << packet.longitud
                << std::setw(40) << packet.data << std::endl;
        }

        buffer.clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main() {
    pcap_if_t* allDevs;
    pcap_if_t* dev;
    pcap_t* handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    int devNum;

    if (pcap_findalldevs(&allDevs, errbuf) == -1) {
        std::cerr << "Error al encontrar dispositivos: " << errbuf << std::endl;
        return -1;
    }

    int i = 0;
    for (dev = allDevs; dev != nullptr; dev = dev->next) {
        std::cout << ++i << ". " << (dev->description ? dev->description : "Sin descripción") << " (" << dev->name << ")" << std::endl;
    }

    std::cout << "Ingresa el número del dispositivo a utilizar: ";
    std::cin >> devNum;

    dev = allDevs;
    for (int j = 0; j < devNum - 1; j++) {
        dev = dev->next;
    }

    handle = pcap_open_live(dev->name, BUFSIZ, 1, 1000, errbuf);
    if (!handle) {
        std::cerr << "No se puede abrir el dispositivo: " << errbuf << std::endl;
        pcap_freealldevs(allDevs);
        return -1;
    }

    std::cout << "Capturando tráfico de red en " << dev->description << "..." << std::endl;

    std::vector<PacketData> packetBuffer;
    pcap_loop(handle, 0, packetHandler, reinterpret_cast<u_char*>(&packetBuffer));

    pcap_freealldevs(allDevs);
    pcap_close(handle);
    return 0;
}
