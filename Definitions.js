/* Type: Room */

	function Room(ID, tempValues, lumensValues, percentageValues){
		this.ID = ID;
		this.tempValues = tempValues;
		this.lumensValues = lumensValues;
		this.percentageValues = percentageValues;
	}

	Room.prototype.changeTempValueAtIndex = function (index, newTemp){
		this.tempValues[index] = newTemp;
	}

/* ---------- */	


