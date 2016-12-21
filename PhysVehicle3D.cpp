#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
	body->setUserPointer(this);
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);
		wheel.color = Black;
		wheel.Render();
	}
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	//chassis1
	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();
	//chassis 2
	Cube chassis2(info.chassis2_size.x, info.chassis2_size.y, info.chassis2_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis2.transform);
	btVector3 offset2(info.chassis2_offset.x, info.chassis2_offset.y, info.chassis2_offset.z);
	offset2 = offset2.rotate(q.getAxis(), q.getAngle());

	chassis2.transform.M[12] += offset2.getX();
	chassis2.transform.M[13] += offset2.getY();
	chassis2.transform.M[14] += offset2.getZ();
	//Sticks
	Cube stick(info.stick_size.x, info.stick_size.y, info.stick_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&stick.transform);
	btVector3 s_offset(info.stick_offset.x, info.stick_offset.y, info.stick_offset.z);
	s_offset = s_offset.rotate(q.getAxis(), q.getAngle());

	stick.transform.M[12] += s_offset.getX();
	stick.transform.M[13] += s_offset.getY();
	stick.transform.M[14] += s_offset.getZ();

	Cube stick2(info.stick2_size.x, info.stick2_size.y, info.stick2_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&stick2.transform);
	btVector3 s_offset2(info.stick2_offset.x, info.stick2_offset.y, info.stick2_offset.z);
	s_offset2 = s_offset2.rotate(q.getAxis(), q.getAngle());

	stick2.transform.M[12] += s_offset2.getX();
	stick2.transform.M[13] += s_offset2.getY();
	stick2.transform.M[14] += s_offset2.getZ();

	//Back winger
	Cube backWinger(info.backWinger_size.x, info.backWinger_size.y, info.backWinger_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&backWinger.transform);
	btVector3 backWinger_offset(info.backWinger_offset.x, info.backWinger_offset.y, info.backWinger_offset.z);
	backWinger_offset = backWinger_offset.rotate(q.getAxis(), q.getAngle());

	backWinger.transform.M[12] += backWinger_offset.getX();
	backWinger.transform.M[13] += backWinger_offset.getY();
	backWinger.transform.M[14] += backWinger_offset.getZ();

	//Lights
	Cube frontLight(info.frontLight_size.x, info.frontLight_size.y, info.frontLight_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&frontLight.transform);
	btVector3 frontLight_offset(info.frontLight_offset.x, info.frontLight_offset.y, info.frontLight_offset.z);
	frontLight_offset = frontLight_offset.rotate(q.getAxis(), q.getAngle());

	frontLight.transform.M[12] += frontLight_offset.getX();
	frontLight.transform.M[13] += frontLight_offset.getY();
	frontLight.transform.M[14] += frontLight_offset.getZ();

	Cube frontLight2(info.frontLight2_size.x, info.frontLight2_size.y, info.frontLight2_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&frontLight2.transform);
	btVector3 frontLight2_offset(info.frontLight2_offset.x, info.frontLight2_offset.y, info.frontLight2_offset.z);
	frontLight2_offset = frontLight2_offset.rotate(q.getAxis(), q.getAngle());

	frontLight2.transform.M[12] += frontLight2_offset.getX();
	frontLight2.transform.M[13] += frontLight2_offset.getY();
	frontLight2.transform.M[14] += frontLight2_offset.getZ();

	//////RENDERS//////
	chassis.color = Grey;
	chassis.Render();
	chassis2.color = Blue;
	chassis2.Render();
	stick.Render();
	stick2.Render();
	backWinger.color = Red;
	backWinger.Render();
	frontLight.color = Yellow;
	frontLight.Render();
	frontLight2.color = Yellow;
	frontLight2.Render();
	
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}