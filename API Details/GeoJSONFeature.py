from fastapi import FastAPI, HTTPException
from pydantic import BaseModel, Field
from motor.motor_asyncio import AsyncIOMotorClient
import os
from dotenv import load_dotenv
from typing import Dict, List

load_dotenv()

app = FastAPI()

mongo_URI = os.getenv("MONGO_URI")
if not mongo_URI:
    raise ValueError("The MONGO_URI environment variable is not set.")

client = AsyncIOMotorClient(mongo_URI)
db = client.GeoJsonFeature
collection = db.Test

class GeoJSONGeometry(BaseModel):
    type: str
    coordinates: List[float]

class GeoJSONFeature(BaseModel):
    type: str = Field(default="Feature")
    geometry: GeoJSONGeometry
    properties: Dict[str, str]

@app.post("/data")
async def create_data(data: GeoJSONFeature):
    # Check for invalid coordinates
    if data.geometry.coordinates == [0, 0]:
        raise HTTPException(status_code=400, detail="Invalid data: Coordinates cannot be [0, 0]")
    
    try:
        result = await collection.insert_one(data.dict())
        return {"message": "Data saved", "id": str(result.inserted_id)}
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Error saving data: {str(e)}")

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
