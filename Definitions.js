/* Type: Room */

	function Room(ID, tempValues, lumensValues, startValues, endValues, brightness, correctedTempValues){
		this.ID = ID;
		this.tempValues = tempValues;
		this.lumensValues = lumensValues;
		this.startValues = startValues;
		this.endValues = endValues;
		this.lightOn = true;
		this.brightness = brightness;

		this.correctedTempValues = correctedTempValues;		
		this.brightnessChanged = false;
		this.roomChanged = false;
		
		this.lightsCompensating = false;
		this.lightsDegraded = false;
		
		this.names = [];
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
	
	Room.prototype.changeLightOn = function (newLightOn){
		this.lightOn = newLightOn;
	}
	
	Room.prototype.changeBrightness = function (index, newBright){
		this.brightness = newBright;
	}
	
	Room.prototype.changeCorrectedTempValueAtIndex = function (index, newCorrectedTemp){
		this.correctedTempValues[index] = newCorrectedTemp;
	}
	
	Room.prototype.changeBrightnessChanged = function (newChanged){
		this.brightnessChanged = newChanged;
	}
	
	Room.prototype.changeRoomChanged = function (newChanged){
		this.roomChanged = newChanged;
	}
	
	Room.prototype.changeLightsCompensating = function (newCompensating){
		this.lightsCompensating = newCompensating;
	}
	
	Room.prototype.changeLightsDegraded = function (newDegraded){
		this.lightsDegraded = newDegraded;
	}
	
module.exports = Room;
/* ---------- */	


