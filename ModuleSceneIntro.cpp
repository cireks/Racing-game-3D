#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "ModulePlayer.h"

#define ROTATE_Y vec3(0,1,0)
#define ROTATE_Z vec3(0,0,1)
#define RECTA_CYLINDERS 43

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	
//	App->audio->PlayMusic("Game/Music/BloodyTears.ogg", 1);
	
	
	num_checkpoints = 0;
	
	timer.Start();
	onetlap.Start();
	

	App->camera->Move(vec3(-50.0f, 15.0f, -20.0f));
	App->camera->LookAt(vec3(-50, 0, 5));

	CreateCircuit();

	//HELIX
	int max_x = 115, min_x = 10;
	Cube helix(25, 1, 4.5);
	Cylinder D_holder(0.3, 3);
	D_holder.SetRotation(90, vec3(0, 0, 1));
	PhysBody3D* new_body = NULL;
	PhysBody3D* aux_body = NULL;
	helix.SetPos(min_x + (max_x / 2 - (helix.size.x / 2)), 12.7, 135);
	D_holder.SetPos(min_x + (max_x / 2), 12.7, 135);
	new_body = App->physics->AddBody(helix, 100);
	aux_body = App->physics->AddBody(D_holder, 0);


	btHingeConstraint* constraint = App->physics->AddConstraintHinge(*new_body, *aux_body,
		vec3{ 0, 0, 0 }, vec3{ 0, 0, 0 }, vec3{ 0, 1, 0 }, vec3{ 0, 1, 0 }, true);

	constraint->enableAngularMotor(true, 3, 250);
	constraint->setLimit(1, 0);

	doors.add(new_body);

	max_x = 35; min_x = 10;
	Cube helix1(25, 1, 4.5);
	Cylinder D_holder1(0.3, 3);
	D_holder1.SetRotation(90, vec3(0, 0, 1));
	new_body = NULL;
	aux_body = NULL;
	helix1.SetPos(min_x + (max_x / 2 - (helix.size.x / 2)), 12.7, 135);
	D_holder1.SetPos(min_x + (max_x / 2), 12.7, 135);
	new_body = App->physics->AddBody(helix1, 100);
	aux_body = App->physics->AddBody(D_holder1, 0);

	btHingeConstraint* constraint1 = App->physics->AddConstraintHinge(*new_body, *aux_body,
		vec3{ 0, 0, 0 }, vec3{ 0, 0, 0 }, vec3{ 0, 1, 0 }, vec3{ 0, 1, 0 }, true);

	constraint1->enableAngularMotor(true, 2, 250);
	constraint1->setLimit(1, 0);

	doors.add(new_body);
	//RAMPS
	Cube Rampa(15, 0.2, 25);
	max_x = 100; min_x = 4;
	Rampa.SetPos(min_x + (max_x / 2), 1.2, -128);
	Rampa.SetRotation(20, { 0, 0, -1 });
	new_body = App->physics->AddBody(Rampa, 0);
	ramps.add(new_body);
	Cube Rampa1(15, 0.2, 25);
	max_x = 150; min_x = 4;
	Rampa1.SetPos(min_x + (max_x / 2), 1.2, -128);
	Rampa1.SetRotation(20, { 0, 0, -1 });
	new_body = App->physics->AddBody(Rampa1, 0);
	ramps.add(new_body);

	//BALLS
	Sphere ball(2);
	ball.SetPos(-47, 1.2, 62);
	ball.color = Black;
	new_body = App->physics->AddBody(ball, 0);
	balls.add(new_body);
	Sphere ball1(2);
	ball1.SetPos(-54, 1.2, 84);
	ball1.color = Black;
	new_body = App->physics->AddBody(ball1, 0);
	balls.add(new_body);
	Sphere ball2(2);
	ball2.SetPos(-49, 1.2, 100);
	ball2.color = Black;
	new_body = App->physics->AddBody(ball2, 0);
	balls.add(new_body);
	Sphere ball3(2);
	ball3.SetPos(-54, 1.2, 45);
	ball3.color = Black;
	new_body = App->physics->AddBody(ball3, 0);
	balls.add(new_body);

	//CHECKPOINTS
	Cube init_sensor(23, 0.3, 2);
	init_sensor.SetPos(-50, 1, -10);
	init_sensor.color = Black;
	meta = App->physics->AddBody(init_sensor, App->scene_intro, true, 0);
	meta->type = tcheckpoint;
	Cube helix_sensor(2, 0.3, 22);
	helix_sensor.SetPos(88, 1, 137.5f);
	helix_sensor.color = Red;
	checkpoint1 = App->physics->AddBody(helix_sensor, App->scene_intro, true, 0);
	checkpoint1->type = tcheckpoint;
	Cube ramp_sensor(2, 20, 22);
	ramp_sensor.SetPos(-22, 1, -129);
	ramp_sensor.color = Red;
	checkpoint2 = App->physics->AddBody(ramp_sensor, App->scene_intro, true, 0);
	checkpoint2->type = tcheckpoint;

	//SPEEDBOOSTS
	Cube Speedzone(8, 0.2, 10);
	Speedzone.SetPos(130, 1, 25);
	Speedzone.color = Yellow;
	speedboost1 = App->physics->AddBody(Speedzone, App->scene_intro, true, 0);
	speedboost1->type = tspeedbost;

	Cube Speedzone1(8, 0.2, 10);
	Speedzone1.SetPos(138, 1, 85);
	Speedzone1.color = Yellow;
	speedboost2 = App->physics->AddBody(Speedzone1, App->scene_intro, true, 0);
	speedboost2->type = tspeedbost;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = false;
	p.wire = false;
	p.color = Yellow;
	p.Render();

	//checkpoints
	Cube init_sensor(23, 0.3, 2);
	init_sensor.SetPos(-50, 1, -10);
	init_sensor.color = Black;
	init_sensor.Render();
	Cube helix_sensor(2, 0.3, 23);
	helix_sensor.SetPos(88, 1, 138.5f);
	helix_sensor.color = Red;
	helix_sensor.Render();
	Cube ramp_sensor(2, 0.3, 25);
	ramp_sensor.SetPos(-22, 1, -129);
	ramp_sensor.color = Red;
	ramp_sensor.Render();

	Cube Speedzone(8, 0.1, 10);
	Speedzone.SetPos(130, 1, 25);
	Speedzone.color = Yellow;
	Speedzone.Render();

	Cube Speedzone1(8, 0.1, 10);
	Speedzone1.SetPos(138, 1, 85);
	Speedzone1.color = Yellow;
	Speedzone1.Render();

	//obstacles

	Cube helix(25, 1, 4.5);
	for (p2List_item<PhysBody3D*>* item = doors.getFirst(); item; item = item->next) {
		item->data->GetTransform(helix.transform.M);
		helix.color = Grey;
		helix.Render();
	}
	Cube Rampa(15, 0.2, 25);
	for (p2List_item<PhysBody3D*>* item = ramps.getFirst(); item; item = item->next) {
		item->data->GetTransform(Rampa.transform.M);
		Rampa.color = Grey;
		Rampa.Render();
	}
	Sphere ball(2);
	for (p2List_item<PhysBody3D*>* item = balls.getFirst(); item; item = item->next) {
		item->data->GetTransform(ball.transform.M);
		ball.color = Black;
		ball.Render();
	}

	
	//circuit
	Cube floor(1000, 1000, 1000);
	floor.SetPos(0, 0, 0);
	floor.Scale(1, 0, 1);
	floor.color = Grey;
	floor.Render();
	
	//RECTA PRINCIPAL
	Cube rectaLeft(2, 7, 140);
	rectaLeft.SetPos(-37, 3.5f, 20);
	rectaLeft.Render();

	Cube rectaRight(2, 7, 140);
	rectaRight.SetPos(-63, 3.5f, 20);
	rectaRight.Render();

	//CORBA ESQUERRA LEFT

	Cylinder c1(1, 7);
	c1.SetPos(-37, 3.5f, 91);
	c1.SetRotation(90, ROTATE_Z);
	
	Cylinder c2(1, 10);
	c2.SetPos(-37, 5, 93.5f);
	c2.SetRotation(90, ROTATE_Z);
	
	Cylinder c3(1, 13);
	c3.SetPos(-37, 6.5f, 96);
	c3.SetRotation(90, ROTATE_Z);

	Cylinder c11(1, 20);
	c11.SetPos(-37, 10, 98.5f);
	c11.SetRotation(90, ROTATE_Z);
	
	Cylinder c12(1, 23);
	c12.SetPos(-37, 11.5f, 101);
	c12.SetRotation(90, ROTATE_Z);
	
	Cylinder c13(1, 23);
	c13.SetPos(-37, 11.5f, 104);
	c13.SetRotation(90, ROTATE_Z);
	
	Cylinder c14(1, 23);
	c14.SetPos(-37, 11.5f, 106.5f);
	c14.SetRotation(90, ROTATE_Z);
	
	Cylinder c15(1, 23);
	c15.SetPos(-37, 11.5f, 109);
	c15.SetRotation(90, ROTATE_Z);
	
	Cylinder c16(1, 23);
	c16.SetPos(-37, 11.5f, 111.5f);
	c16.SetRotation(90, ROTATE_Z);
	
	Cylinder c4(1, 23);
	c4.SetPos(-37, 11.5f, 114);
	c4.SetRotation(90, ROTATE_Z);
	
	Cylinder c5(1, 23);
	c5.SetPos(-36.5f, 11.5, 117);
	c5.SetRotation(90, ROTATE_Z);
	
	Cylinder c6(1, 23);
	c6.SetPos(-34, 11.5f, 120);
	c6.SetRotation(90, ROTATE_Z);
	
	Cylinder c7(1, 20);
	c7.SetPos(-31.5f, 10, 123);
	c7.SetRotation(90, ROTATE_Z);
	
	Cylinder c8(1, 13);
	c8.SetPos(-29, 6.5f, 126);
	c8.SetRotation(90, ROTATE_Z);
	
	Cylinder c9(1, 10);
	c9.SetPos(-26.5, 5, 126);
	c9.SetRotation(90, ROTATE_Z);
	
	Cylinder c10(1, 7);
	c10.SetPos(-24, 3.5f, 126);
	c10.SetRotation(90, ROTATE_Z);
	c1.Render();
	c2.Render();
	c3.Render();
	c4.Render();
	c5.Render();
	c6.Render();
	c7.Render();
	c8.Render();
	c9.Render();
	c10.Render();
	c11.Render();
	c12.Render();
	c13.Render();
	c14.Render();
	c15.Render();
	c16.Render();
	
	//CORBA ESQUERRA RIGHT
	Cylinder c17(1, 7);
	c17.SetPos(-63, 3.5f, 91);
	c17.SetRotation(90, ROTATE_Z);
	
	Cylinder c18(1, 10);
	c18.SetPos(-63, 5, 93.5f);
	c18.SetRotation(90, ROTATE_Z);
	
	Cylinder c19(1, 13);
	c19.SetPos(-63, 6.5f, 96);
	c19.SetRotation(90, ROTATE_Z);
	
	Cylinder c20(1, 20);
	c20.SetPos(-63, 10, 98.5f);
	c20.SetRotation(90, ROTATE_Z);
	
	Cylinder c21(1, 23);
	c21.SetPos(-63, 11.5f, 101);
	c21.SetRotation(90, ROTATE_Z);
	
	Cylinder c22(1, 23);
	c22.SetPos(-63, 11.5f, 104);
	c22.SetRotation(90, ROTATE_Z);
	
	Cylinder c23(1, 23);
	c23.SetPos(-63, 11.5f, 107);
	c23.SetRotation(90, ROTATE_Z);
	
	Cylinder c24(1, 23);
	c24.SetPos(-63, 11.5f, 110);
	c24.SetRotation(90, ROTATE_Z);
	
	Cylinder c25(1, 23);
	c25.SetPos(-63, 11.5f, 113);
	c25.SetRotation(90, ROTATE_Z);
	
	Cylinder c26(1, 23);
	c26.SetPos(-63, 11.5f, 116);
	c26.SetRotation(90, ROTATE_Z);
	
	Cylinder c27(1, 23);
	c27.SetPos(-63, 11.5f, 118.5f);
	c27.SetRotation(90, ROTATE_Z);
	
	Cylinder c28(1, 23);
	c28.SetPos(-63, 11.5f, 121);
	c28.SetRotation(90, ROTATE_Z);
	
	Cylinder c29(1, 23);
	c29.SetPos(-63, 11.5f, 123.5f);
	c29.SetRotation(90, ROTATE_Z);
	
	Cylinder c30(1, 23);
	c30.SetPos(-62, 11.5f, 126);
	c30.SetRotation(90, ROTATE_Z);
	
	Cylinder c42(1, 23);
	c42.SetPos(-62, 11.5f, 129.5f);
	c42.SetRotation(90, ROTATE_Z);
	
	Cylinder c41(1, 23);
	c41.SetPos(-60.5f, 11.5f, 133);
	c41.SetRotation(90, ROTATE_Z);
	
	Cylinder c31(1, 23);
	c31.SetPos(-59, 11.5f, 136);
	c31.SetRotation(90, ROTATE_Z);
	
	Cylinder c32(1, 23);
	c32.SetPos(-56, 11.5f, 139);
	c32.SetRotation(90, ROTATE_Z);
	
	Cylinder c33(1, 23);
	c33.SetPos(-53, 11.5f, 142);
	c33.SetRotation(90, ROTATE_Z);
	
	Cylinder c34(1, 23);
	c34.SetPos(-50, 11.5f, 145);
	c34.SetRotation(90, ROTATE_Z);
	
	Cylinder c35(1, 23);
	c35.SetPos(-47, 11.5f, 148);
	c35.SetRotation(90, ROTATE_Z);
	
	Cylinder c36(1, 23);
	c36.SetPos(-44, 11.5f, 151);
	c36.SetRotation(90, ROTATE_Z);
	
	Cylinder c37(1, 20);
	c37.SetPos(-41.5f, 10, 151);
	c37.SetRotation(90, ROTATE_Z);
	
	Cylinder c38(1, 13);
	c38.SetPos(-39, 6.5f, 151);
	c38.SetRotation(90, ROTATE_Z);
	
	Cylinder c39(1, 10);
	c39.SetPos(-36.5f, 5, 151);
	c39.SetRotation(90, ROTATE_Z);
	
	Cylinder c40(1, 7);
	c40.SetPos(-34, 3.5f, 151);
	c40.SetRotation(90, ROTATE_Z);
	
	c17.Render();
	c18.Render();
	c19.Render();
	c20.Render();
	c21.Render();
	c22.Render();
	c23.Render();
	c24.Render();
	c25.Render();
	c26.Render();
	c27.Render();
	c28.Render();
	c29.Render();
	c30.Render();
	c31.Render();
	c32.Render();
	c33.Render();
	c34.Render();
	c35.Render();
	c36.Render();
	c37.Render();
	c38.Render();
	c39.Render();
	c40.Render();
	c41.Render();
	c42.Render();

	//RECTA ESQUERRA
	Cube rEsquerraLeft(2, 7, 21);
	rEsquerraLeft.SetPos(-12.5f, 3.5f, 126);
	rEsquerraLeft.SetRotation(90, ROTATE_Y);
	rEsquerraLeft.Render();
	Cube rEsquerraRight(2, 7, 30);
	rEsquerraRight.SetPos(-17.4f, 3.5f, 151);
	rEsquerraRight.SetRotation(90, ROTATE_Y);
	rEsquerraRight.Render();

	//AMPLIACIO
	Cube ampDown(2, 7, 10);
	ampDown.SetPos(-3, 3.5f, 120);
	ampDown.Render();
	Cube ampUp(2, 7, 10);
	ampUp.SetPos(-3, 3.5f, 157);
	ampUp.Render();

	//RECTA AMPLIADA
	Cube rectaDown(2, 7, 90);
	rectaDown.SetPos(41, 3.5f, 115);
	rectaDown.SetRotation(90, ROTATE_Y);
	rectaDown.Render();
	Cube rectaUp(2, 7, 90);
	rectaUp.SetPos(41, 3.5f, 162);
	rectaUp.SetRotation(90, ROTATE_Y);
	rectaUp.Render();

	//REDUCCIO
	Cube redDown(2, 7, 10);
	redDown.SetPos(87, 3.5f, 120);
	redDown.Render();
	Cube redUp(2, 7, 10);
	redUp.SetPos(87, 3.5f, 157);
	redUp.Render();

	//SORTIDA TRAM AMPLIAT
	Cube tramUp(2, 7, 60);
	tramUp.SetPos(117, 3.5f, 151);
	tramUp.SetRotation(90, ROTATE_Y);
	tramUp.Render();
	Cube tramDown(2, 7, 30);
	tramDown.SetPos(102, 3.5f, 126);
	tramDown.SetRotation(90, ROTATE_Y);
	tramDown.Render();

	//RECTA CAP ABAIX 
	Cube rAbaixLeft(2, 7, 150);
	rAbaixLeft.SetPos(146, 3.5f, 75);
	rAbaixLeft.Render();
	Cube rAbaixRight(2, 7, 120);
	rAbaixRight.SetPos(116, 3.5f, 65);
	rAbaixRight.Render();

	//DIAGONAL1
	Cube diagonalLeft(2, 7, 50);
	diagonalLeft.SetPos(133, 3.5f, -22);
	diagonalLeft.SetRotation(33, ROTATE_Y);
	diagonalLeft.Render();
	Cube diagonalRight(2, 7, 55);
	diagonalRight.SetPos(101, 3.5f, -19);
	diagonalRight.SetRotation(33, ROTATE_Y);
	diagonalRight.Render();

	//DIAGONAL2
	Cube diagonalLeft2(2, 7, 50);
	diagonalLeft2.SetPos(135.4f, 3.5f, -61);
	diagonalLeft2.SetRotation(-45, ROTATE_Y);
	diagonalLeft2.Render();
	Cube diagonalRight2(2, 7, 55);
	diagonalRight2.SetPos(105, 3.5f, -63);
	diagonalRight2.SetRotation(-45, ROTATE_Y);
	diagonalRight2.Render();

	//CANTONADA ABAIX-ESQUERRA
	Cube corner(2, 7, 60);
	corner.SetPos(153.3, 3.5f, -109.4);
	corner.Render();

	Cube corner2(2, 7, 33);
	corner2.SetPos(124.4f, 3.5f, -100);
	corner2.Render();

	//RECTA CAP A LA DRETA

	Cylinder r1(1, 20);
	r1.SetPos(-2.5f, 10, -116);
	r1.SetRotation(90, ROTATE_Z);
	
	Cylinder r2(1, 13);
	r2.SetPos(-5, 6.5f, -116);
	r2.SetRotation(90, ROTATE_Z);
	
	Cylinder r3(1, 7);
	r3.SetPos(-7.5f, 3.5f, -116);
	r3.SetRotation(90, ROTATE_Z);
	
	Cylinder r4(1, 20);
	r4.SetPos(-2.5f, 10, -142);
	r4.SetRotation(90, ROTATE_Z);
	
	Cylinder r5(1, 13);
	r5.SetPos(-5, 6.5f, -142);
	r5.SetRotation(90, ROTATE_Z);
	
	Cylinder r6(1, 7);
	r6.SetPos(-7.5f, 3.5f, -142);
	r6.SetRotation(90, ROTATE_Z);
	

	Cylinder r7(1, 20);
	r7.SetPos(107.5f, 10, -116);
	r7.SetRotation(90, ROTATE_Z);
	
	Cylinder r8(1, 20);
	r8.SetPos(110, 10, -116);
	r8.SetRotation(90, ROTATE_Z);
	
	Cylinder r9(1, 13);
	r9.SetPos(112.5f, 6.5f, -116);
	r9.SetRotation(90, ROTATE_Z);
	
	Cylinder r13(1, 13);
	r13.SetPos(115, 6.5f, -116);
	r13.SetRotation(90, ROTATE_Z);
	
	Cylinder r14(1, 7);
	r14.SetPos(117.5f, 3.5f, -116);
	r14.SetRotation(90, ROTATE_Z);
	
	Cylinder r15(1, 7);
	r15.SetPos(120, 3.5f, -116);
	r15.SetRotation(90, ROTATE_Z);
	
	Cylinder r16(1, 7);
	r16.SetPos(122.3f, 3.5f, -116);
	r16.SetRotation(90, ROTATE_Z);
	Cylinder r10(1, 20);
	r10.SetPos(142, 10, -142);
	r10.SetRotation(90, ROTATE_Z);
	
	Cylinder r11(1, 20);
	r11.SetPos(144.5f, 10, -142);
	r11.SetRotation(90, ROTATE_Z);
	
	Cylinder r12(1, 13);
	r12.SetPos(147, 6.5f, -142);
	r12.SetRotation(90, ROTATE_Z);
	
	Cylinder r17(1, 13);
	r17.SetPos(149.5f, 6.5f, -142);
	r17.SetRotation(90, ROTATE_Z);
	
	Cylinder r18(1, 13);
	r18.SetPos(151.8f, 6.5f, -142);
	r18.SetRotation(90, ROTATE_Z);
	
	Cylinder r19(1, 13);
	r19.SetPos(153.3f, 6.5f, -142);
	r19.SetRotation(90, ROTATE_Z);
	
	r1.Render();
	r2.Render();
	r3.Render();
	r4.Render();
	r5.Render();
	r6.Render();
	r7.Render();
	r8.Render();
	r9.Render();
	r10.Render();
	r11.Render();
	r12.Render();
	r13.Render();
	r14.Render();
	r15.Render();
	r16.Render();
	r17.Render();
	r18.Render();
	r19.Render();



	for (int i = 0; i < 14; ++i)
	{
		Cylinder r(1, 23);
		r.SetPos(107.5f + (i*2.5f), 11.5f, -142);
		r.SetRotation(90, ROTATE_Z);
		r.Render();
	}
	

	for (int i = 0; i < RECTA_CYLINDERS; i++)
	{
		Cylinder c100Above(1, 23);
		c100Above.SetPos(0 + (i*2.5f), 11.5f, -116);
		c100Above.SetRotation(90, ROTATE_Z);
		c100Above.Render();

		Cylinder c100Below(1, 23);
		c100Below.SetPos(0 + (i*2.5f), 11.5f, -142);
		c100Below.SetRotation(90, ROTATE_Z);
		c100Below.Render();

	}

	for (int i = 0; i < 7; i++)
	{
		Cylinder k(1, 7);
		k.SetPos(-25 + (i*2.5f), 3.5f, -116);
		k.SetRotation(90, ROTATE_Z);
		k.Render();
	}
	for (int i = 0; i < 15; i++)
	{
		Cylinder p(1, 7);
		p.SetPos(-45.5f + (i*2.5f), 3.5f, -142);
		p.SetRotation(90, ROTATE_Z);
		p.Render();
	}


	//DIAGONAL ULTIMA
	for (int i = 0; i < 19; i++)
	{
		Cylinder d(1, 7);
		d.SetPos(-37, 3.5f, -52.5f - (i*2.5f));
		d.SetRotation(90, ROTATE_Z);
		d.Render();
	}
	for (int i = 0; i < 7; i++)
	{
		Cylinder s(1, 7);
		s.SetPos(-28 - (i*1.5f), 3.5f, -116 + (i*2.5f));
		s.SetRotation(90, ROTATE_Z);
		s.Render();
	}
	for (int i = 0; i < 25; i++)
	{
		Cylinder a(1, 7);
		a.SetPos(-63, 3.5f, -52.5f - (i*2.5f));
		a.SetRotation(90, ROTATE_Z);
		a.Render();
	}
	for (int i = 0; i < 11; i++)
	{
		Cylinder j(1, 7);
		j.SetPos(-48 - (i*1.5f), 3.5f, -140.5 + (i*2.5f));
		j.SetRotation(90, ROTATE_Z);
		j.Render();
	}

	
	
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1->type == tvehicle)
	{
		if (body2 == checkpoint1 && num_checkpoints == 0)
		{
			num_checkpoints = 1;
		}
		if (body2 == checkpoint2 && num_checkpoints == 1)
		{
			num_checkpoints = 2;
		}
		if (body2 == meta && num_checkpoints == 2)
		{
			if(App->player->lap_num <3)
			{
				App->player->lap_num += 1;
				if (App->player->lap_num == 1) {
					onetlap.Stop();
					lastlap = onetlap.Read() / 1000;
					App->player->best_lap = onetlap.Read() / 1000;
					seclap.Start();
				}
				if (App->player->lap_num == 2) {
					seclap.Stop();
					if ((seclap.Read() / 1000) < App->player->best_lap)
						App->player->best_lap = seclap.Read() / 1000;
					lastlap = seclap.Read() / 1000;
					trilap.Start();

				}
				if (App->player->lap_num == 3) {
					trilap.Stop();
					if ((trilap.Read() /1000) < App->player->best_lap)
						App->player->best_lap = trilap.Read() / 1000;
					lastlap = trilap.Read() / 1000;
					timer.Stop();
					App->player->win = true;
				}
			}
			
			num_checkpoints = 0;
		}
		if (body2 == speedboost1)
		{
			body1->body->applyCentralImpulse(btVector3(0, 0, -200));
		}
		if (body2 == speedboost2)
		{
			body1->body->applyCentralImpulse(btVector3(0, 0, -200));
		}
	}
	
}

void ModuleSceneIntro::CreateCircuit()
{

	//RECTA PRINCIPAL
	Cube rectaLeft(2, 7, 140);
	rectaLeft.SetPos(-37, 3.5f, 20);
	App->physics->AddBody(rectaLeft,0);
	Cube rectaRight(2, 7, 140);
	rectaRight.SetPos(-63, 3.5f, 20);
	App->physics->AddBody(rectaRight,0);
	
	//CORBA ESQUERRA LEFT

	Cylinder c1(1, 7);
	c1.SetPos(-37, 3.5f, 91);
	c1.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c1, 0);
	Cylinder c2(1, 10);
	c2.SetPos(-37, 5, 93.5f);
	c2.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c2, 0);
	Cylinder c3(1, 13);
	c3.SetPos(-37, 6.5f, 96);
	c3.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c3, 0);
	Cylinder c11(1, 20);
	c11.SetPos(-37, 10, 98.5f);
	c11.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c11, 0);
	Cylinder c12(1, 23);
	c12.SetPos(-37, 11.5f, 101);
	c12.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c12, 0);
	Cylinder c13(1, 23);
	c13.SetPos(-37, 11.5f, 104);
	c13.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c13, 0);
	Cylinder c14(1, 23);
	c14.SetPos(-37, 11.5f, 106.5f);
	c14.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c14, 0);
	Cylinder c15(1, 23);
	c15.SetPos(-37, 11.5f, 109);
	c15.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c15, 0);
	Cylinder c16(1, 23);
	c16.SetPos(-37, 11.5f, 111.5f);
	c16.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c16, 0);
	Cylinder c4(1, 23);
	c4.SetPos(-37, 11.5f, 114);
	c4.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c4, 0);
	Cylinder c5(1, 23);
	c5.SetPos(-36.5f, 11.5, 117);
	c5.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c5, 0);
	Cylinder c6(1, 23);
	c6.SetPos(-34, 11.5f, 120);
	c6.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c6, 0);
	Cylinder c7(1, 20);
	c7.SetPos(-31.5f, 10, 123);
	c7.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c7, 0);
	Cylinder c8(1, 13);
	c8.SetPos(-29, 6.5f, 126);
	c8.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c8, 0);
	Cylinder c9(1, 10);
	c9.SetPos(-26.5, 5, 126);
	c9.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c9, 0);
	Cylinder c10(1, 7);
	c10.SetPos(-24, 3.5f, 126);
	c10.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c10, 0);

	//CORBA ESQUERRA RIGHT
	Cylinder c17(1, 7);
	c17.SetPos(-63, 3.5f, 91);
	c17.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c17, 0);
	Cylinder c18(1, 10);
	c18.SetPos(-63, 5, 93.5f);
	c18.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c18, 0);
	Cylinder c19(1, 13);
	c19.SetPos(-63, 6.5f, 96);
	c19.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c19, 0);
	Cylinder c20(1, 20);
	c20.SetPos(-63, 10, 98.5f);
	c20.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c20, 0);
	Cylinder c21(1, 23);
	c21.SetPos(-63, 11.5f, 101);
	c21.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c21, 0);
	Cylinder c22(1, 23);
	c22.SetPos(-63, 11.5f, 104);
	c22.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c22, 0);
	Cylinder c23(1, 23);
	c23.SetPos(-63, 11.5f, 107);
	c23.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c23, 0);
	Cylinder c24(1, 23);
	c24.SetPos(-63, 11.5f, 110);
	c24.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c24, 0);
	Cylinder c25(1, 23);
	c25.SetPos(-63, 11.5f, 113);
	c25.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c25, 0);
	Cylinder c26(1, 23);
	c26.SetPos(-63, 11.5f, 116);
	c26.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c26, 0);
	Cylinder c27(1, 23);
	c27.SetPos(-63, 11.5f, 118.5f);
	c27.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c27, 0);
	Cylinder c28(1, 23);
	c28.SetPos(-63, 11.5f, 121);
	c28.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c28, 0);
	Cylinder c29(1, 23);
	c29.SetPos(-63, 11.5f, 123.5f);
	c29.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c29, 0);
	Cylinder c30(1, 23);
	c30.SetPos(-62, 11.5f, 126);
	c30.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c30, 0);
	Cylinder c42(1, 23);
	c42.SetPos(-62, 11.5f, 129.5f);
	c42.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c42, 0);
	Cylinder c41(1, 23);
	c41.SetPos(-60.5f, 11.5f, 133);
	c41.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c41, 0);
	Cylinder c31(1, 23);
	c31.SetPos(-59, 11.5f, 136);
	c31.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c31, 0);
	Cylinder c32(1, 23);
	c32.SetPos(-56, 11.5f, 139);
	c32.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c32, 0);
	Cylinder c33(1, 23);
	c33.SetPos(-53, 11.5f, 142);
	c33.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c33, 0);
	Cylinder c34(1, 23);
	c34.SetPos(-50, 11.5f, 145);
	c34.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c34, 0);
	Cylinder c35(1, 23);
	c35.SetPos(-47, 11.5f, 148);
	c35.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c35, 0);
	Cylinder c36(1, 23);
	c36.SetPos(-44, 11.5f, 151);
	c36.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c36, 0);
	Cylinder c37(1, 20);
	c37.SetPos(-41.5f, 10, 151);
	c37.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c37, 0);
	Cylinder c38(1, 13);
	c38.SetPos(-39, 6.5f, 151);
	c38.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c38, 0);
	Cylinder c39(1, 10);
	c39.SetPos(-36.5f, 5, 151);
	c39.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c39, 0);
	Cylinder c40(1, 7);
	c40.SetPos(-34, 3.5f, 151);
	c40.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(c40, 0);


	//RECTA ESQUERRA
	Cube rEsquerraLeft(2, 7, 21);
	rEsquerraLeft.SetPos(-12.5f,3.5f , 126);
	rEsquerraLeft.SetRotation(90, ROTATE_Y);
	App->physics->AddBody(rEsquerraLeft, 0);
	Cube rEsquerraRight(2, 7, 30);
	rEsquerraRight.SetPos(-17.4f, 3.5f, 151);
	rEsquerraRight.SetRotation(90, ROTATE_Y);
	App->physics->AddBody(rEsquerraRight, 0);

	//AMPLIACIO
	Cube ampDown(2, 7, 10);
	ampDown.SetPos(-3, 3.5f, 120);
	App->physics->AddBody(ampDown, 0);
	Cube ampUp(2, 7, 10);
	ampUp.SetPos(-3, 3.5f, 157);
	App->physics->AddBody(ampUp, 0);

	//RECTA AMPLIADA
	Cube rectaDown(2, 7, 90);
	rectaDown.SetPos(41, 3.5f, 115);
	rectaDown.SetRotation(90, ROTATE_Y);
	App->physics->AddBody(rectaDown, 0);
	Cube rectaUp(2, 7, 90);
	rectaUp.SetPos(41, 3.5f, 162);
	rectaUp.SetRotation(90, ROTATE_Y);
	App->physics->AddBody(rectaUp, 0);

	//REDUCCIO
	Cube redDown(2, 7, 10);
	redDown.SetPos(87, 3.5f, 120);
	App->physics->AddBody(redDown, 0);
	Cube redUp(2, 7, 10);
	redUp.SetPos(87, 3.5f, 157);
	App->physics->AddBody(redUp, 0);

	//SORTIDA TRAM AMPLIAT
	Cube tramUp(2, 7, 60);
	tramUp.SetPos(117, 3.5f, 151);
	tramUp.SetRotation(90, ROTATE_Y);
	App->physics->AddBody(tramUp, 0);
	Cube tramDown(2, 7, 30);
	tramDown.SetPos(102, 3.5f, 126);
	tramDown.SetRotation(90, ROTATE_Y);
	App->physics->AddBody(tramDown, 0);

	//RECTA CAP ABAIX 
	Cube rAbaixLeft(2, 7, 150);
	rAbaixLeft.SetPos(146, 3.5f, 75);
	App->physics->AddBody(rAbaixLeft, 0);
	Cube rAbaixRight(2, 7, 120);
	rAbaixRight.SetPos(116, 3.5f, 65);
	App->physics->AddBody(rAbaixRight, 0);

	//DIAGONAL1
	Cube diagonalLeft(2, 7, 50);
	diagonalLeft.SetPos(133, 3.5f, -22);
	diagonalLeft.SetRotation(33, ROTATE_Y);
	App->physics->AddBody(diagonalLeft, 0);
	Cube diagonalRight(2, 7, 55);
	diagonalRight.SetPos(101, 3.5f, -19);
	diagonalRight.SetRotation(33, ROTATE_Y);
	App->physics->AddBody(diagonalRight, 0);

	//DIAGONAL2
	Cube diagonalLeft2(2, 7, 50);
	diagonalLeft2.SetPos(135.4f, 3.5f, -61);
	diagonalLeft2.SetRotation(-45, ROTATE_Y);
	App->physics->AddBody(diagonalLeft2, 0);
	Cube diagonalRight2(2, 7, 55);
	diagonalRight2.SetPos(105, 3.5f, -63);
	diagonalRight2.SetRotation(-45, ROTATE_Y);
	App->physics->AddBody(diagonalRight2, 0);

	//CANTONADA ABAIX-ESQUERRA
	Cube corner(2, 7, 60);
	corner.SetPos(153.3,3.5f,-109.4);
	App->physics->AddBody(corner, 0);

	Cube corner2(2, 7, 33);
	corner2.SetPos(124.4f, 3.5f, -100);
	App->physics->AddBody(corner2, 0);

	//RECTA CAP A LA DRETA

	Cylinder r1(1, 20);
	r1.SetPos(-2.5f, 10, -116);
	r1.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r1, 0);
	Cylinder r2(1, 13);
	r2.SetPos(-5, 6.5f, -116);
	r2.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r2, 0);
	Cylinder r3(1, 7);
	r3.SetPos(-7.5f, 3.5f, -116);
	r3.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r3, 0);
	Cylinder r4(1, 20);
	r4.SetPos(-2.5f, 10, -142);
	r4.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r4, 0);
	Cylinder r5(1, 13);
	r5.SetPos(-5, 6.5f, -142);
	r5.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r5, 0);
	Cylinder r6(1, 7);
	r6.SetPos(-7.5f, 3.5f, -142);
	r6.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r6, 0);

	Cylinder r7(1, 20);
	r7.SetPos(107.5f, 10, -116);
	r7.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r7, 0);
	Cylinder r8(1, 20);
	r8.SetPos(110, 10, -116);
	r8.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r8, 0);
	Cylinder r9(1, 13);
	r9.SetPos(112.5f, 6.5f, -116);
	r9.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r9, 0);
	Cylinder r13(1, 13);
	r13.SetPos(115, 6.5f, -116);
	r13.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r13, 0);
	Cylinder r14(1, 7);
	r14.SetPos(117.5f, 3.5f, -116);
	r14.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r14, 0);
	Cylinder r15(1, 7);
	r15.SetPos(120, 3.5f, -116);
	r15.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r15, 0);
	Cylinder r16(1, 7);
	r16.SetPos(122.3f, 3.5f, -116);
	r16.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r16, 0);
	for (int i = 0; i < 14; ++i)
	{
		Cylinder r(1, 23);
		r.SetPos(107.5f + (i*2.5f), 11.5f, -142);
		r.SetRotation(90, ROTATE_Z);
		App->physics->AddBody(r, 0);
	}
	Cylinder r10(1, 20);
	r10.SetPos(142, 10, -142);
	r10.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r10, 0);
	Cylinder r11(1, 20);
	r11.SetPos(144.5f, 10, -142);
	r11.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r11, 0);
	Cylinder r12(1, 13);
	r12.SetPos(147, 6.5f, -142);
	r12.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r12, 0);
	Cylinder r17(1, 13);
	r17.SetPos(149.5f, 6.5f, -142);
	r17.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r17, 0);
	Cylinder r18(1, 13);
	r18.SetPos(151.8f, 6.5f, -142);
	r18.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r18, 0);
	Cylinder r19(1, 13);
	r19.SetPos(153.3f, 6.5f, -142);
	r19.SetRotation(90, ROTATE_Z);
	App->physics->AddBody(r19, 0);

	
	

	for (int i = 0; i < RECTA_CYLINDERS; i++)
	{
		Cylinder c100Above(1, 23);
		c100Above.SetPos(0+(i*2.5f), 11.5f, -116);
		c100Above.SetRotation(90, ROTATE_Z);
		App->physics->AddBody(c100Above, 0);
		Cylinder c100Below(1, 23);
		c100Below.SetPos(0 + (i*2.5f), 11.5f, -142);
		c100Below.SetRotation(90, ROTATE_Z);
		App->physics->AddBody(c100Below, 0);

	}

	for (int i = 0; i < 7; i++)
	{
		Cylinder k(1, 7);
		k.SetPos(-25 + (i*2.5f), 3.5f, -116);
		k.SetRotation(90, ROTATE_Z);
		App->physics->AddBody(k, 0);
	}
	for (int i = 0; i < 15; i++)
	{
		Cylinder p(1, 7);
		p.SetPos(-45.5f + (i*2.5f), 3.5f, -142);
		p.SetRotation(90, ROTATE_Z);
		App->physics->AddBody(p, 0);
	}


	//DIAGONAL ULTIMA
	for (int i = 0; i < 19; i++)
	{
		Cylinder d(1, 7);
		d.SetPos(-37, 3.5f, -52.5f - (i*2.5f));
		d.SetRotation(90, ROTATE_Z);
		App->physics->AddBody(d, 0);
	}
	for (int i = 0; i < 7; i++)
	{
		Cylinder s(1, 7);
		s.SetPos(-28 - (i*1.5f), 3.5f, -116+(i*2.5f));
		s.SetRotation(90, ROTATE_Z);
		App->physics->AddBody(s, 0);
	}
	for (int i = 0; i < 25; i++)
	{
		Cylinder a(1, 7);
		a.SetPos(-63, 3.5f, -52.5f - (i*2.5f));
		a.SetRotation(90, ROTATE_Z);
		App->physics->AddBody(a, 0);
	}
	for (int i = 0; i < 11; i++)
	{
		Cylinder j(1, 7);
		j.SetPos(-48 - (i*1.5f), 3.5f, -140.5 + (i*2.5f));
		j.SetRotation(90, ROTATE_Z);
		App->physics->AddBody(j, 0);
	}
	

}

