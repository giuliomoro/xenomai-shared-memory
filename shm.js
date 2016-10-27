const mmap = require("mmap-io")
const fs = require("fs")
const util = require('util');
// hardcode the defines from stru.h
//#define SHM_NAME "/myshared"
const SHM_NAME = "/dev/shm/myshared"
//#define STRU_NAME_SIZE 20
const STRU_NAME_SIZE=20;
//#define STRU_VALUES_SIZE 5
const STRU_VALUES_SIZE=5;

const fd = fs.openSync(SHM_NAME, "r")
console.log("fd: ", fd)
var size = fs.statSync(SHM_NAME).size // no idea why this requires SHM_NAME: the specs require "int fd" as the only argument
console.log("File size: ", size)
var rx_prot = mmap.PROT_READ | mmap.PROT_EXECUTE
var priv = mmap.MAP_SHARED
buf = mmap.map(size, rx_prot, priv, fd)

console.log("buffer size: ", buf.length)

console.log("name: ", buf.toString("utf8", 0, STRU_NAME_SIZE))
const floatSize = 4;
const valuesStart = STRU_NAME_SIZE; // the values[] array is STRU_NAME_SIZE bytes into the array (assuming the C structure was allocated wihtout padding bytes)
for(var n = valuesStart; n < size; n+=floatSize){
	console.log(util.format("values[%d]: %d", (n-valuesStart)/floatSize , buf.readFloatLE(n)));
}

