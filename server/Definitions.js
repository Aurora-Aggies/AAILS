/* Type: Room */

	function Room(ID, tempValues, lumensValues, percentageValues, brightness, compensation){
		this.ID = ID;
		this.tempValues = tempValues;
		this.lumensValues = lumensValues;
		this.percentageValues = percentageValues;
		this.brightness = brightness;
		this.compensation = compensation;
	}

	Room.prototype.changeTempValueAtIndex = function (index, newTemp){
		this.tempValues[index] = newTemp;
	}

	Room.prototype.changeLumensValueAtIndex = function (index, newLumen){
		this.lumensValues[index] = newLumen;
	}
	
	Room.prototype.changeCompensationValueAtIndex = function (index, newComp){
		this.compensation[index] = newComp;
	}


module.exports = Room;
/* ---------- */	


