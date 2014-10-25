# -------------------------------------------------
# Project created by QtCreator 2010-11-01T12:06:34
# -------------------------------------------------
QT += sql \
    xml
TARGET = Almacen
TEMPLATE = app
SOURCES += ventanaLogin.cpp \
    principal.cpp \
    main.cpp \
    configuracion.cpp \
    conexionBD.cpp \
    clsImagen.cpp \
    clsconfiguracion.cpp \
    Operarios/operario.cpp \
    Operarios/operariosficha.cpp \
    Operarios/operarioslista.cpp \
    Operarios/clsimprimirpartes.cpp \
    Centros/centros.cpp \
    Centros/centro.cpp \
    Centros/centrosficha.cpp \
    Centros/centroslista.cpp \
    Vehiculos/vehiculos.cpp \
    Vehiculos/vehiculosficha.cpp \
    Vehiculos/vehiculo.cpp \
    Vehiculos/clsimprimirvehiculos.cpp \
    Kits/kitslista.cpp \
    Kits/kits.cpp \
    Kits/kitficha.cpp \
    Kits/kit.cpp \
    Movimientos/movimientosrecepcion.cpp \
    Movimientos/movimientospedido.cpp \
    Movimientos/movimientoslistas.cpp \
    Movimientos/movimientosenvio.cpp \
    Movimientos/movimientosentrada.cpp \
    Movimientos/movimientosdevoluciones.cpp \
    Movimientos/movimientos.cpp \
    Movimientos/movimientosstock.cpp \
    Movimientos/movimientossalidas.cpp \
    Productos/certificaciones.cpp \
    Productos/productosficha.cpp \
    Productos/categoriasficha.cpp \
    Productos/familiasficha.cpp \
    Productos/categoriaslista.cpp \
    Productos/familiaslista.cpp \
    Productos/producto.cpp \
    Productos/clsimprimirproductos.cpp \
    Operarios/partestrabajodet.cpp \
    Operarios/partestrabajoficha.cpp \
    Centros/clsimprimircentro.cpp \
    Kits/clsimprimirkits.cpp \
    Operarios/clsimprimiroperarios.cpp \
    Movimientos/clsimprimirmovimientos.cpp \
    Productos/listas.cpp \
    Operarios/operariospartes.cpp \
    Productos/productoslista.cpp
HEADERS += ventanaLogin.h \
    principal.h \
    configuracion.h \
    conexionBD.h \
    clsImagen.h \
    clsconfiguracion.h \
    Operarios/operario.h \
    Operarios/operariosficha.h \
    Operarios/operarioslista.h \
    Operarios/clsimprimirpartes.h \
    Centros/centros.h \
    Centros/centro.h \
    Centros/centrosficha.h \
    Centros/centroslista.h \
    Vehiculos/vehiculosficha.h \
    Vehiculos/vehiculo.h \
    Vehiculos/clsimprimirvehiculos.h \
    Vehiculos/vehiculos.h \
    Kits/kitslista.h \
    Kits/kits.h \
    Kits/kitficha.h \
    Kits/kit.h \
    Movimientos/movimientosrecepcion.h \
    Movimientos/movimientospedido.h \
    Movimientos/movimientoslistas.h \
    Movimientos/movimientosenvio.h \
    Movimientos/movimientosentrada.h \
    Movimientos/movimientosdevoluciones.h \
    Movimientos/movimientos.h \
    Movimientos/movimientosstock.h \
    Movimientos/movimientossalidas.h \
    Productos/certificaciones.h \
    Productos/productosficha.h \
    Productos/categoriasficha.h \
    Productos/familiasficha.h \
    Productos/categoriaslista.h \
    Productos/familiaslista.h \
    Productos/producto.h \
    Productos/clsimprimirproductos.h \
    Operarios/partestrabajoficha.h \
    Operarios/partestrabajodet.h \
    Centros/clsimprimircentro.h \
    Kits/clsimprimirkits.h \
    Movimientos/clsimprimirmovimientos.h \
    Operarios/clsimprimiroperarios.h \
    Productos/listas.h \
    Operarios/operariospartes.h \
    Productos/productoslista.h
FORMS += principal.ui \
    ventanaLogin.ui \
    configuracion.ui \
    Operarios/operariosficha.ui \
    Operarios/operarioslista.ui \
    Centros/centros.ui \
    Centros/centrosficha.ui \
    Centros/centroslista.ui \
    Vehiculos/vehiculos.ui \
    Vehiculos/vehiculosficha.ui \
    Kits/kitslista.ui \
    Kits/kits.ui \
    Kits/kitficha.ui \
    Movimientos/movimientosrecepcion.ui \
    Movimientos/movimientospedido.ui \
    Movimientos/movimientoslistas.ui \
    Movimientos/movimientosenvio.ui \
    Movimientos/movimientosentrada.ui \
    Movimientos/movimientosdevoluciones.ui \
    Movimientos/movimientosstock.ui \
    Movimientos/movimientossalidas.ui \
    Productos/certificaciones.ui \
    Productos/productosficha.ui \
    Productos/categoriasficha.ui \
    Productos/familiasficha.ui \
    Productos/categoriaslista.ui \
    Productos/familiaslista.ui \
    Operarios/partestrabajoficha.ui \
    Operarios/partestrabajodet.ui \
    Productos/listas.ui \
    Operarios/operariospartes.ui \
    Productos/productoslista.ui
RESOURCES += iconos.qrc
TRANSLATIONS = Almacen_es_es.ts \
    Almacen_gb_en.ts
CODECFORTR = UTF-8
CODECFORSRC = UTF-8
