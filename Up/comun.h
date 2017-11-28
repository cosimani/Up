#ifndef COMUN_H
#define COMUN_H

#include <QObject>
#include <QStringList>

/**
 * @brief The Comun class es una clase que mantiene informacion comun para ser utilizada de cualquier lado
 */
class Comun : public QObject
{
    Q_OBJECT

private:
    static Comun *instance;
    explicit Comun( QObject *parent = NULL );

public:
    enum SizePlantilla { SIZE15x10, SIZE21x15 };
    enum Impresion { IMPRIMIR, PDF, NOPDF_NOPRINTER };
    enum TipoEvento { NUEVO, EXISTENTE, AUN_NO_ELEGIDO, PROYECTAR };
    enum Internet { ONLINE, OFFLINE };

    static Comun *getInstance();
    ~Comun();

    void iniciar();

    bool getOnline() const;
    void setOnline(bool value);

    QString getHashTag() const;
    void setHashTag(const QString &value);

    QStringList getHashTags();
    void setHashTags(const QStringList &value);

    SizePlantilla getSizeHoja() const;
    void setSizeHoja(const SizePlantilla &value);

    Impresion getImpresion() const;
    void setImpresion(const Impresion &value);

    QString getArchivoPlantilla() const;
    void setArchivoPlantilla(const QString &value);

    int getTimerScroll() const;
    void setTimerScroll(int value);

    int getTimerActualizarPagina() const;
    void setTimerActualizarPagina(int value);

    int getTimerDir() const;
    void setTimerDir(int value);

    int getTimerimpresiones() const;
    void setTimerimpresiones(int value);

    int getTimerEscena() const;
    void setTimerEscena(int value);

    TipoEvento getTipoEvento() const;
    void setTipoEvento(const TipoEvento &value);

    Internet getInternet() const;
    void setInternet(const Internet &value);

private:

    /**
     * @brief hashTag Mantiene el HashTag que se esta usando actualmente.
     * Puede ser porque se crea uno nuevo o porque se eligio alguno existente.
     */
    QString hashTag;

    /**
     * @brief hashTags Ni bien arranca el programa, se consulta a la base de datos para mantener en este
     * objeto, todos los hastag que estan registrados en la base.
     */
    QStringList hashTags;

    /**
     * @brief sizePlantilla Por defecto se inicia en 15x10
     */
    SizePlantilla sizeHoja;

    /**
     * @brief impresion PDF o Imprimir. Por defecto PDF.
     */
    Impresion impresion;

    /**
     * @brief The TipoEvento enum Un evento nuevo siempre sera online
     * El estado de AUN_NO_ELEGIDO es para cuando aun no se selecciono en Principal (Este es valor por defecto)
     */
    TipoEvento tipoEvento;

    /**
     * @brief internet Para definir online (descargar fotos de Instagram) u offline para no descargar.
     * Por defecto sera offline hasta tanto se elija en ConfEvento.
     */
    Internet internet;

    /**
     * @brief archivoPlantilla Es el nombre del archivo plantilla elegido.
     */
    QString archivoPlantilla;

    /**
     * @brief timerScroll Milisegundos para hacer scroll en instagram (defecto 10000)
     */
    int timerScroll;

    /**
     * @brief timerActualizarPagina Milisegundo para recargar la pagina de instagram (defecto 60000)
     */
    int timerActualizarPagina;

    /**
     * @brief timerDir Miliseg para buscar nuevas fotos descargadas en el directorio de esta fiesta (defecto 5000)
     */
    int timerDir;

    /**
     * @brief timerimpresiones Milisegundos para tirar cada impresion (ya sea en PDF o printer) (defecto 10000)
     */
    int timerimpresiones;

    /**
     * @brief timerEscena Milisegundos para actualizar la foto en la escena (defecto 10000)
     */
    int timerEscena;

signals:
    /**
     * @brief signal_timerActualizado Se emite cuando alguno de los timers se actualiza.
     * Son 5 timers: timerScroll, timerActualizarPagina, timerDir, timerimpresiones y timerEscena
     */
    void signal_timerActualizado();

public slots:


};

#endif // COMUN_H
