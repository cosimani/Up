#include "comun.h"
#include "adminDB.h"
#include <QDebug>

Comun *Comun::instance = NULL;

Comun::Comun(QObject *parent) : QObject(parent),
                                sizeHoja( SIZE15x10 ),
                                impresion( NOPDF_NOPRINTER ),
                                tipoEvento( AUN_NO_ELEGIDO ),
                                internet( OFFLINE ),
                                timerScroll( 10000 ),
                                timerActualizarPagina( 60000 ),
                                timerDir( 5000 ),
                                timerimpresiones( 10000 ),
                                timerEscena( 10000 )
{

}

Comun *Comun::getInstance()
{
    if( !instance )
    {
        instance = new Comun();
    }
    return instance;
}

Comun::~Comun()
{
    delete instance;
}

/**
 * @brief Comun::iniciar Conecta la base y carga todo los datos que sean necesarios en Comun.
 */
void Comun::iniciar()
{
    qDebug() << "La base se conecto" << AdminDB::getInstance()->conectar("../db/baseFotoYa.sqlite");
    QStringList hashTagEnBase = AdminDB::getInstance()->getHashTags();
    this->setHashTags( hashTagEnBase );
    qDebug() << "HashTags en la base y cagados en Comun" << hashTagEnBase;

}

QString Comun::getHashTag() const
{
    return hashTag;
}

void Comun::setHashTag(const QString &value)
{
    hashTag = value;
}

QStringList Comun::getHashTags()
{
    this->hashTags.clear();
    this->hashTags = AdminDB::getInstance()->getHashTags();

    return hashTags;
}

void Comun::setHashTags(const QStringList &value)
{
    hashTags = value;
}

Comun::SizePlantilla Comun::getSizeHoja() const
{
    return sizeHoja;
}

void Comun::setSizeHoja(const SizePlantilla &value)
{
    sizeHoja = value;
}

Comun::Impresion Comun::getImpresion() const
{
    return impresion;
}

void Comun::setImpresion(const Impresion &value)
{
    impresion = value;
}

QString Comun::getArchivoPlantilla() const
{
    return archivoPlantilla;
}

void Comun::setArchivoPlantilla(const QString &value)
{
    archivoPlantilla = value;
}

int Comun::getTimerScroll() const
{
    return timerScroll;
}

void Comun::setTimerScroll(int value)
{
    timerScroll = qMax( 5000, value );
    emit signal_timerActualizado();
}

int Comun::getTimerActualizarPagina() const
{
    return timerActualizarPagina;
}

void Comun::setTimerActualizarPagina(int value)
{
    timerActualizarPagina = qMax( 10000, value );
    emit signal_timerActualizado();
}

int Comun::getTimerDir() const
{
    return timerDir;
}

void Comun::setTimerDir(int value)
{
    timerDir = qMax( 2000, value );
    emit signal_timerActualizado();
}

int Comun::getTimerimpresiones() const
{
    return timerimpresiones;
}

void Comun::setTimerimpresiones(int value)
{
    timerimpresiones = qMax( 8000, value );
    emit signal_timerActualizado();
}

int Comun::getTimerEscena() const
{
    return timerEscena;
}

void Comun::setTimerEscena(int value)
{
    timerEscena = qMax( 3000, value );
    emit signal_timerActualizado();
}

Comun::TipoEvento Comun::getTipoEvento() const
{
    return tipoEvento;
}

void Comun::setTipoEvento(const TipoEvento &value)
{
    tipoEvento = value;
}

Comun::Internet Comun::getInternet() const
{
    return internet;
}

void Comun::setInternet(const Internet &value)
{
    internet = value;
}

