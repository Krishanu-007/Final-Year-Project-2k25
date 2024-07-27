from fastapi import FastAPI, HTTPException
from pydantic import BaseModel,Field
from motor.motor_asyncio import AsyncIOMotorClient
import os

app = FastAPI()

mongo_uri=os.getenv("MONGO_URI")

if not mongo_uri:
	raise ValueError("ENV not set")

client=AsyncIOMotorClient(mongo_uri)
db=client.geoJsonTest
collection=db.locationTest

class locData(BaseModel):
	common_id: str
	latitude: float
	longitude: float

@app.post("/testPost")
async def send_data(data: locData):
	if data.latitude == 0.0 or data.longitude == 0.0:
		raise HTTPException(status_code=400,detail="Invalid coodinates")

	geoJsonData={"common_id": data.common_id,"type": "Point","coordinates": [data.latitude, data.longitude]}

	try:
		result=await collection.insert_one(geoJsonData)
		return {"message": "Data saved", "id": str(result.inserted_id)}
	except Exception as e:
		raise HTTPException(status_code=500, detail=f"Error saving data: {str(e)}")


if __name__ == "__main__":
	import uvicorn
	uvicorn.run(app,host="0.0.0.0",port=8000)


