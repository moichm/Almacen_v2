CREATE DATABASE  IF NOT EXISTS `almacen` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `almacen`;
-- MySQL dump 10.13  Distrib 5.5.16, for Win32 (x86)
--
-- Host: localhost    Database: almacen
-- ------------------------------------------------------
-- Server version	5.5.21

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `linea_salidas`
--

DROP TABLE IF EXISTS `linea_salidas`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `linea_salidas` (
  `idSalida` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `idProducto` varchar(20) NOT NULL,
  `cantidad` int(11) NOT NULL,
  `fecha_salida` date NOT NULL,
  PRIMARY KEY (`idSalida`,`fecha_salida`,`idProducto`),
  KEY `salidprod` (`idProducto`),
  CONSTRAINT `sal` FOREIGN KEY (`idSalida`) REFERENCES `salidas` (`idSalida`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `salidprod` FOREIGN KEY (`idProducto`) REFERENCES `productos` (`idProducto`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `linea_salidas`
--

LOCK TABLES `linea_salidas` WRITE;
/*!40000 ALTER TABLE `linea_salidas` DISABLE KEYS */;
INSERT INTO `linea_salidas` VALUES (1,'4widvf3934',1,'2012-10-04'),(2,'4widvf3934',2,'2012-10-04');
/*!40000 ALTER TABLE `linea_salidas` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `familias`
--

DROP TABLE IF EXISTS `familias`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `familias` (
  `idFamilia` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(150) NOT NULL,
  `frec_certif` int(11) NOT NULL DEFAULT '0',
  `certificable` tinyint(1) NOT NULL,
  `limite_entrega` int(11) DEFAULT '0',
  `limitado` tinyint(1) NOT NULL,
  PRIMARY KEY (`idFamilia`),
  UNIQUE KEY `idFamilia` (`idFamilia`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `familias`
--

LOCK TABLES `familias` WRITE;
/*!40000 ALTER TABLE `familias` DISABLE KEYS */;
INSERT INTO `familias` VALUES (1,'Bolardos',0,0,0,0),(2,'Balizas',3,1,0,0),(3,'Camisetas',0,0,4,1);
/*!40000 ALTER TABLE `familias` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `envios`
--

DROP TABLE IF EXISTS `envios`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `envios` (
  `idEnvio` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `idCentroOrigen` bigint(20) unsigned NOT NULL,
  `idCentroDestino` bigint(20) unsigned NOT NULL,
  `fecha_creacion` date NOT NULL,
  `Comentario` text,
  PRIMARY KEY (`idEnvio`),
  UNIQUE KEY `idEnvio` (`idEnvio`),
  KEY `fk_Envios_1` (`idCentroOrigen`),
  KEY `fk_Envios_2` (`idCentroDestino`),
  CONSTRAINT `fk_Envios_1` FOREIGN KEY (`idCentroOrigen`) REFERENCES `centros` (`idCentro`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `fk_Envios_2` FOREIGN KEY (`idCentroDestino`) REFERENCES `centros` (`idCentro`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `envios`
--

LOCK TABLES `envios` WRITE;
/*!40000 ALTER TABLE `envios` DISABLE KEYS */;
INSERT INTO `envios` VALUES (1,1,2,'2012-10-16','');
/*!40000 ALTER TABLE `envios` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kits`
--

DROP TABLE IF EXISTS `kits`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `kits` (
  `idKit` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(150) NOT NULL,
  PRIMARY KEY (`idKit`),
  UNIQUE KEY `idKit` (`idKit`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `kits`
--

LOCK TABLES `kits` WRITE;
/*!40000 ALTER TABLE `kits` DISABLE KEYS */;
INSERT INTO `kits` VALUES (2,'kit de prueba 1'),(3,'Kit de prueba de bolardos');
/*!40000 ALTER TABLE `kits` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `centros`
--

DROP TABLE IF EXISTS `centros`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `centros` (
  `idCentro` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `nombre` varchar(150) NOT NULL,
  `ciudad` varchar(150) NOT NULL,
  `Direccion` varchar(300) NOT NULL,
  PRIMARY KEY (`idCentro`),
  UNIQUE KEY `codigo` (`idCentro`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `centros`
--

LOCK TABLES `centros` WRITE;
/*!40000 ALTER TABLE `centros` DISABLE KEYS */;
INSERT INTO `centros` VALUES (1,'Centro ciudad','Cádiz','porteria de capuchinos nº5'),(2,'La isla','San Fernando','');
/*!40000 ALTER TABLE `centros` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `perm_user`
--

DROP TABLE IF EXISTS `perm_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `perm_user` (
  `idUsuario` varchar(20) NOT NULL,
  `pass` varchar(45) NOT NULL,
  `prodcon` tinyint(1) NOT NULL,
  `prodmod` tinyint(1) NOT NULL,
  `vehicon` tinyint(1) NOT NULL,
  `vehimod` tinyint(1) NOT NULL,
  `opcon` tinyint(1) NOT NULL,
  `opmod` tinyint(1) NOT NULL,
  `centcon` tinyint(1) NOT NULL,
  `centmod` tinyint(1) NOT NULL,
  `movcon` tinyint(1) NOT NULL,
  `movmod` tinyint(1) NOT NULL,
  `kitcon` tinyint(1) NOT NULL,
  `kitmod` tinyint(1) NOT NULL,
  `conf` tinyint(1) NOT NULL,
  PRIMARY KEY (`idUsuario`),
  UNIQUE KEY `idUsuario` (`idUsuario`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `perm_user`
--

LOCK TABLES `perm_user` WRITE;
/*!40000 ALTER TABLE `perm_user` DISABLE KEYS */;
INSERT INTO `perm_user` VALUES ('admin','4dm1n',1,1,1,1,1,1,1,1,1,1,1,1,1),('moi','m01',1,1,1,1,1,1,1,1,1,1,1,1,0);
/*!40000 ALTER TABLE `perm_user` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mov_vehiculos`
--

DROP TABLE IF EXISTS `mov_vehiculos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mov_vehiculos` (
  `matricula` varchar(10) NOT NULL,
  `fecha_mov` date NOT NULL,
  `idCentro` bigint(20) unsigned NOT NULL,
  `km_finales` decimal(10,0) NOT NULL,
  PRIMARY KEY (`matricula`,`fecha_mov`),
  KEY `movidco` (`idCentro`),
  CONSTRAINT `movidco` FOREIGN KEY (`idCentro`) REFERENCES `centros` (`idCentro`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `movmat` FOREIGN KEY (`matricula`) REFERENCES `vehiculos` (`matricula`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mov_vehiculos`
--

LOCK TABLES `mov_vehiculos` WRITE;
/*!40000 ALTER TABLE `mov_vehiculos` DISABLE KEYS */;
INSERT INTO `mov_vehiculos` VALUES ('ca-1221-ve','2012-06-26',1,0),('ca-1221-ve','2012-06-27',2,0),('ca-1221-ve','2012-06-28',1,0),('ca-1221-ve','2012-07-11',2,0);
/*!40000 ALTER TABLE `mov_vehiculos` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `linea_entradas`
--

DROP TABLE IF EXISTS `linea_entradas`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `linea_entradas` (
  `idSalida` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `idProducto` varchar(20) NOT NULL,
  `devueltas` int(11) NOT NULL,
  `perdidas` int(11) NOT NULL,
  `fecha_salida` date NOT NULL,
  `fecha_entrada` date NOT NULL,
  PRIMARY KEY (`idSalida`,`fecha_salida`,`fecha_entrada`,`idProducto`),
  KEY `entidprod` (`idProducto`),
  CONSTRAINT `entidprod` FOREIGN KEY (`idProducto`) REFERENCES `productos` (`idProducto`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `entprod` FOREIGN KEY (`idSalida`) REFERENCES `salidas` (`idSalida`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `linea_entradas`
--

LOCK TABLES `linea_entradas` WRITE;
/*!40000 ALTER TABLE `linea_entradas` DISABLE KEYS */;
INSERT INTO `linea_entradas` VALUES (2,'4widvf3934',1,0,'2012-10-04','2012-11-08'),(2,'4widvf3934',1,0,'2012-10-04','2012-11-09');
/*!40000 ALTER TABLE `linea_entradas` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `operarios`
--

DROP TABLE IF EXISTS `operarios`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `operarios` (
  `nif` varchar(9) NOT NULL,
  `nombre` varchar(20) NOT NULL,
  `apellidos` varchar(300) NOT NULL,
  `fecha_nacimiento` date NOT NULL,
  `direccion` varchar(300) NOT NULL,
  `email` varchar(100) DEFAULT NULL,
  `ccc` varchar(20) NOT NULL,
  `idCentro` bigint(20) unsigned NOT NULL,
  `imagen` varchar(300) DEFAULT NULL,
  PRIMARY KEY (`nif`),
  UNIQUE KEY `nif` (`nif`),
  KEY `fk_Operarios_1` (`idCentro`),
  CONSTRAINT `fk_Operarios_1` FOREIGN KEY (`idCentro`) REFERENCES `centros` (`idCentro`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `operarios`
--

LOCK TABLES `operarios` WRITE;
/*!40000 ALTER TABLE `operarios` DISABLE KEYS */;
INSERT INTO `operarios` VALUES ('76083214l','Alejandro','Chaves Mota','1987-07-12','porteria de capuchinos nº 5 - bajo izqda','alejandro.chm@hotmail.com','22221111334444444444',2,'C:/Documents and Settings/Moi/Mis documentos/moi/cadiz cf/02.jpg'),('76083215c','Moises','Chaves Mota','1986-06-13','porteria de capuchinos nº 5 - bajo izqda','moises.chm@hotmail.com','11112222334444444444',1,'C:/Documents and Settings/Moi/Mis documentos/moi/foto_yo.jpg');
/*!40000 ALTER TABLE `operarios` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `linea_partes`
--

DROP TABLE IF EXISTS `linea_partes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `linea_partes` (
  `idParte` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `tipohora` enum('Normal','Extra') NOT NULL,
  `proyecto` varchar(100) NOT NULL,
  `tarea` varchar(100) NOT NULL DEFAULT '',
  `idOperario` varchar(9) NOT NULL DEFAULT '',
  `numhoras` decimal(10,0) NOT NULL,
  PRIMARY KEY (`idParte`,`idOperario`,`proyecto`,`tarea`,`tipohora`),
  KEY `parteidop` (`idOperario`),
  CONSTRAINT `parteidop` FOREIGN KEY (`idOperario`) REFERENCES `operarios` (`nif`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `partelinea` FOREIGN KEY (`idParte`) REFERENCES `partes_trabajo` (`idParte`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `linea_partes`
--

LOCK TABLES `linea_partes` WRITE;
/*!40000 ALTER TABLE `linea_partes` DISABLE KEYS */;
INSERT INTO `linea_partes` VALUES (1,'Normal','qweq','ere','76083214l',6),(1,'Extra','hkbvsdf','sdofh','76083215c',2);
/*!40000 ALTER TABLE `linea_partes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `linea_pedido`
--

DROP TABLE IF EXISTS `linea_pedido`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `linea_pedido` (
  `idPedido` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `idProducto` varchar(20) NOT NULL,
  `fecha_pedido` date NOT NULL,
  `cantidad` int(11) NOT NULL,
  `servido` tinyint(4) NOT NULL,
  PRIMARY KEY (`idPedido`,`idProducto`,`fecha_pedido`),
  KEY `pedidoid` (`idProducto`),
  CONSTRAINT `pedido` FOREIGN KEY (`idPedido`) REFERENCES `pedidos_internos` (`idPedido`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `pedidoidprod` FOREIGN KEY (`idProducto`) REFERENCES `productos` (`idProducto`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `linea_pedido`
--

LOCK TABLES `linea_pedido` WRITE;
/*!40000 ALTER TABLE `linea_pedido` DISABLE KEYS */;
INSERT INTO `linea_pedido` VALUES (1,'4widvf3934','2012-10-16',1,0);
/*!40000 ALTER TABLE `linea_pedido` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `categorias`
--

DROP TABLE IF EXISTS `categorias`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `categorias` (
  `idCategoria` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(150) NOT NULL,
  PRIMARY KEY (`idCategoria`),
  UNIQUE KEY `idCategoria` (`idCategoria`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `categorias`
--

LOCK TABLES `categorias` WRITE;
/*!40000 ALTER TABLE `categorias` DISABLE KEYS */;
INSERT INTO `categorias` VALUES (1,'Luminarias Exterior'),(2,'Luminarias Interior'),(5,'Ropa'),(6,'EPI');
/*!40000 ALTER TABLE `categorias` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `linea_kit`
--

DROP TABLE IF EXISTS `linea_kit`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `linea_kit` (
  `idKit` bigint(20) NOT NULL AUTO_INCREMENT,
  `idProducto` varchar(20) NOT NULL,
  `cantidad` int(11) NOT NULL,
  PRIMARY KEY (`idKit`,`idProducto`),
  KEY `kitidprod` (`idProducto`),
  CONSTRAINT `kitidprod` FOREIGN KEY (`idProducto`) REFERENCES `productos` (`idProducto`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `linea_kit`
--

LOCK TABLES `linea_kit` WRITE;
/*!40000 ALTER TABLE `linea_kit` DISABLE KEYS */;
INSERT INTO `linea_kit` VALUES (2,'1w223121',1),(2,'1w223122',1),(2,'3ew43435g',2),(2,'4widvf3934',2),(3,'1w223121',4),(3,'1w223122',1),(3,'3ew43435g',1),(3,'4widvf3934',1),(4,'4widvf3934',1),(5,'3ew43435g',1),(6,'4widvf3934',1),(7,'3ew43435g',1),(8,'4widvf3934',1),(9,'3ew43435g',1);
/*!40000 ALTER TABLE `linea_kit` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `partes_trabajo`
--

DROP TABLE IF EXISTS `partes_trabajo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `partes_trabajo` (
  `idParte` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `fecha_parte` date NOT NULL,
  `responsable` varchar(9) NOT NULL,
  `descripcion` varchar(300) DEFAULT NULL,
  `idCentro` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`idParte`),
  UNIQUE KEY `idParte` (`idParte`),
  KEY `fk_Partes_Trabajo_1` (`responsable`),
  KEY `fk_Partes_Trabajo_2` (`idCentro`),
  CONSTRAINT `fk_Partes_Trabajo_1` FOREIGN KEY (`responsable`) REFERENCES `operarios` (`nif`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_Partes_Trabajo_2` FOREIGN KEY (`idCentro`) REFERENCES `centros` (`idCentro`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `partes_trabajo`
--

LOCK TABLES `partes_trabajo` WRITE;
/*!40000 ALTER TABLE `partes_trabajo` DISABLE KEYS */;
INSERT INTO `partes_trabajo` VALUES (1,'2012-06-27','76083215c','primer parte de trabajo',1);
/*!40000 ALTER TABLE `partes_trabajo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `telefonos`
--

DROP TABLE IF EXISTS `telefonos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `telefonos` (
  `idOperario` varchar(9) NOT NULL,
  `telefono` varchar(15) NOT NULL,
  PRIMARY KEY (`idOperario`,`telefono`),
  CONSTRAINT `telidop` FOREIGN KEY (`idOperario`) REFERENCES `operarios` (`nif`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `telefonos`
--

LOCK TABLES `telefonos` WRITE;
/*!40000 ALTER TABLE `telefonos` DISABLE KEYS */;
/*!40000 ALTER TABLE `telefonos` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `productos`
--

DROP TABLE IF EXISTS `productos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `productos` (
  `idProducto` varchar(20) NOT NULL,
  `descripcion` varchar(300) NOT NULL,
  `disponible` tinyint(1) NOT NULL,
  `idFamilia` bigint(20) unsigned NOT NULL,
  `idCategoria` bigint(20) unsigned NOT NULL,
  `imagen` varchar(500) DEFAULT NULL,
  PRIMARY KEY (`idProducto`),
  UNIQUE KEY `idProducto_UNIQUE` (`idProducto`),
  KEY `familia` (`idFamilia`),
  KEY `categoria` (`idCategoria`),
  CONSTRAINT `categoria` FOREIGN KEY (`idCategoria`) REFERENCES `categorias` (`idCategoria`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `familia` FOREIGN KEY (`idFamilia`) REFERENCES `familias` (`idFamilia`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `productos`
--

LOCK TABLES `productos` WRITE;
/*!40000 ALTER TABLE `productos` DISABLE KEYS */;
INSERT INTO `productos` VALUES ('1w223121','luminaria de exterior blanca',1,1,1,'C:/Documents and Settings/Moi/Mis documentos/cadiz-de-noche.jpg'),('1w223122','luminaria de exterior roja',1,1,1,''),('3ew43435g','camiseta mangas cortas roja',1,3,5,''),('4widvf3934','baliza de exterior',1,2,1,'C:/Documents and Settings/Moi/Mis documentos/atadecer_en_la_caleta-1.jpg');
/*!40000 ALTER TABLE `productos` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `certificaciones`
--

DROP TABLE IF EXISTS `certificaciones`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `certificaciones` (
  `idProducto` varchar(20) NOT NULL,
  `fecha_certificacion` date NOT NULL,
  `emisor` varchar(150) NOT NULL,
  PRIMARY KEY (`idProducto`,`fecha_certificacion`),
  KEY `fk_Certificaciones_1` (`idProducto`),
  CONSTRAINT `fk_Certificaciones_1` FOREIGN KEY (`idProducto`) REFERENCES `productos` (`idProducto`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `certificaciones`
--

LOCK TABLES `certificaciones` WRITE;
/*!40000 ALTER TABLE `certificaciones` DISABLE KEYS */;
INSERT INTO `certificaciones` VALUES ('4widvf3934','2012-07-11','prueba'),('4widvf3934','2012-11-12','');
/*!40000 ALTER TABLE `certificaciones` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `linea_envios`
--

DROP TABLE IF EXISTS `linea_envios`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `linea_envios` (
  `idEnvio` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `idProducto` varchar(20) NOT NULL,
  `fecha_envio` date NOT NULL,
  `cantidad` int(11) NOT NULL,
  `Recibido` tinyint(4) NOT NULL,
  PRIMARY KEY (`idEnvio`,`idProducto`,`fecha_envio`),
  KEY `envioidprod` (`idProducto`),
  CONSTRAINT `envioidprod` FOREIGN KEY (`idProducto`) REFERENCES `productos` (`idProducto`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `envioprod` FOREIGN KEY (`idEnvio`) REFERENCES `envios` (`idEnvio`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `linea_envios`
--

LOCK TABLES `linea_envios` WRITE;
/*!40000 ALTER TABLE `linea_envios` DISABLE KEYS */;
INSERT INTO `linea_envios` VALUES (1,'1w223121','2012-11-13',1,1),(1,'4widvf3934','2012-11-12',1,1);
/*!40000 ALTER TABLE `linea_envios` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `vehiculos`
--

DROP TABLE IF EXISTS `vehiculos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vehiculos` (
  `matricula` varchar(10) NOT NULL,
  `marca` varchar(100) NOT NULL,
  `modelo` varchar(100) NOT NULL,
  `fecha_matriculacion` date NOT NULL,
  `imagen` varchar(500) DEFAULT NULL,
  PRIMARY KEY (`matricula`),
  UNIQUE KEY `matricula` (`matricula`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `vehiculos`
--

LOCK TABLES `vehiculos` WRITE;
/*!40000 ALTER TABLE `vehiculos` DISABLE KEYS */;
INSERT INTO `vehiculos` VALUES ('ca-1221-ve','Ford','Transit','2010-06-01','C:/Documents and Settings/Moi/Mis documentos/CADIZ2714.jpg');
/*!40000 ALTER TABLE `vehiculos` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `salidas`
--

DROP TABLE IF EXISTS `salidas`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `salidas` (
  `idSalida` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `idCentro` bigint(20) unsigned NOT NULL,
  `proyecto` varchar(100) DEFAULT NULL,
  `tarea` varchar(100) DEFAULT NULL,
  `responsable` varchar(9) NOT NULL,
  `fecha_creacion` date NOT NULL,
  `Comentario` text,
  PRIMARY KEY (`idSalida`),
  UNIQUE KEY `idSalida` (`idSalida`),
  KEY `fk_Salidas_1` (`idCentro`),
  KEY `fk_Salidas_2` (`responsable`),
  CONSTRAINT `fk_Salidas_1` FOREIGN KEY (`idCentro`) REFERENCES `centros` (`idCentro`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `fk_Salidas_2` FOREIGN KEY (`responsable`) REFERENCES `operarios` (`nif`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `salidas`
--

LOCK TABLES `salidas` WRITE;
/*!40000 ALTER TABLE `salidas` DISABLE KEYS */;
INSERT INTO `salidas` VALUES (1,1,'Proyecto de prueba nº1','Tarea de prueba nº1 del proyecto nº1','76083215c','2012-10-04',''),(2,1,'Proyecto 2','tarea de prueba para el proyecto 2','76083214l','2012-10-04','');
/*!40000 ALTER TABLE `salidas` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ped_env_prod`
--

DROP TABLE IF EXISTS `ped_env_prod`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ped_env_prod` (
  `idPedido` bigint(20) unsigned NOT NULL,
  `idEnvio` bigint(20) unsigned NOT NULL,
  `idProducto` varchar(20) NOT NULL,
  `Fecha_Envio` date NOT NULL,
  `Fecha_Pedido` date NOT NULL,
  PRIMARY KEY (`idPedido`,`idEnvio`,`idProducto`,`Fecha_Envio`,`Fecha_Pedido`),
  KEY `ped_env_1` (`idPedido`),
  KEY `ped_env_2` (`idEnvio`),
  KEY `ped_env_3` (`idProducto`),
  CONSTRAINT `ped_env_1` FOREIGN KEY (`idPedido`) REFERENCES `pedidos_internos` (`idPedido`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `ped_env_2` FOREIGN KEY (`idEnvio`) REFERENCES `envios` (`idEnvio`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `ped_env_3` FOREIGN KEY (`idProducto`) REFERENCES `productos` (`idProducto`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ped_env_prod`
--

LOCK TABLES `ped_env_prod` WRITE;
/*!40000 ALTER TABLE `ped_env_prod` DISABLE KEYS */;
/*!40000 ALTER TABLE `ped_env_prod` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pedidos_internos`
--

DROP TABLE IF EXISTS `pedidos_internos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pedidos_internos` (
  `idPedido` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `Fecha_Creacion` date NOT NULL,
  `idCentro` bigint(20) unsigned NOT NULL,
  `Comentario` text,
  PRIMARY KEY (`idPedido`),
  UNIQUE KEY `idPedido` (`idPedido`),
  KEY `pedintcent` (`idCentro`),
  CONSTRAINT `pedintcent` FOREIGN KEY (`idCentro`) REFERENCES `centros` (`idCentro`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pedidos_internos`
--

LOCK TABLES `pedidos_internos` WRITE;
/*!40000 ALTER TABLE `pedidos_internos` DISABLE KEYS */;
INSERT INTO `pedidos_internos` VALUES (1,'2012-10-16',1,'');
/*!40000 ALTER TABLE `pedidos_internos` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `stock`
--

DROP TABLE IF EXISTS `stock`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `stock` (
  `fecha_stock` date NOT NULL,
  `idCentro` bigint(20) unsigned NOT NULL,
  `idProducto` varchar(20) NOT NULL,
  `Origen` enum('Entrada','Salida') NOT NULL,
  `comentario` varchar(500) DEFAULT NULL,
  `Cantidad` int(11) NOT NULL,
  PRIMARY KEY (`fecha_stock`,`idCentro`,`Origen`,`idProducto`),
  KEY `centros_stock` (`idCentro`),
  KEY `prod_stock` (`idProducto`),
  CONSTRAINT `centros_stock` FOREIGN KEY (`idCentro`) REFERENCES `centros` (`idCentro`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `prod_stock` FOREIGN KEY (`idProducto`) REFERENCES `productos` (`idProducto`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `stock`
--

LOCK TABLES `stock` WRITE;
/*!40000 ALTER TABLE `stock` DISABLE KEYS */;
INSERT INTO `stock` VALUES ('2012-10-04',1,'4widvf3934','Entrada','',5),('2012-11-13',1,'1w223121','Entrada','',2);
/*!40000 ALTER TABLE `stock` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-12-12 21:40:03
