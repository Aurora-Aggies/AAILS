/* Type: Room */

	function Room(ID, tempValues, lumensValues, startValues, endValues, brightness, correctedTempValues){
		this.ID = ID;
		this.tempValues = tempValues;
		this.lumensValues = lumensValues;
		this.startValues = startValues;
		this.endValues = endValues;
		this.brightness = brightness;

		this.correctedTempValues = correctedTempValues;		
		this.brightnessChanged = false;
		this.roomChanged = false;
	}

	Room.prototype.changeTempValueAtIndex = function (index, newTemp){
		this.tempValues[index] = newTemp;
	}

	Room.prototype.changeLumensValueAtIndex = function (index, newLumen){
		this.lumensValues[index] = newLumen;
	}
	
	Room.prototype.changeStartValueAtIndex = function (index, newStart){
		this.startValues[index] = newStart;
	}
	
	Room.prototype.changeEndValueAtIndex = function (index, newEnd){
		this.endValues[index] = newEnd;
	}
	
	Room.prototype.changeBrightness = function (index, newBright){
		this.brightness = newBright;
	}
	
	Room.prototype.changeCorrectedTempValueAtIndex = function (index, newTemp){
		this.correctedTempValues[index] = newTemp;
	}
	
	Room.prototype.changeBrightnessChanged = function (newChanged){
		this.brightnessChanged = newChanged;
	}
	
	Room.prototype.changeRoomChanged = function (newChanged){
		this.roomChanged = newChanged;
	}
	
module.exports = Room;
/* ---------- */	


