#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#define ROTATE_Y vec3(0,1,0)

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
	render = false;
	App->camera->Move(vec3(-50.0f, 15.0f, -20.0f));
	App->camera->LookAt(vec3(-50, 0, 5));
	CreateCircuit();
	
	//helice r.o.d
	int max_x = 5, min_x = 0;
	Cube helix(10, 1, 1.5);
	Cylinder D_holder(0.3, 3);
	D_holder.SetRotation(90, vec3(0,0,1));
	PhysBody3D* new_body = NULL;
	PhysBody3D* aux_body = NULL;
	helix.SetPos(min_x + (max_x / 2 - (helix.size.x / 2)), 6.5, 0 );
	D_holder.SetPos(min_x + (max_x / 2), 6.5, 0);
	new_body = App->physics->AddBody(helix, 10);
	aux_body = App->physics->AddBody(D_holder, 0);


	btHingeConstraint* constraint = App->physics->AddConstraintHinge(*new_body, *aux_body,
		vec3{ 0, 0, 0 }, vec3{ 0, 0, 0 }, vec3{ 0, 1, 0 }, vec3{ 0, 1, 0 }, true);

	
		constraint->enableAngularMotor(true, 2, 20);
	
	constraint->setLimit(1, 0);

	doors.add(new_body);

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
	p.axis = true;
	p.Render();
	

	Cube helix(10, 1, 1.5);
	for (p2List_item<PhysBody3D*>* item = doors.getFirst(); item; item = item->next) {
		item->data->GetTransform(helix.transform.M);
		helix.Render();
	}

	//RECTA PRINCIPAL
	Cube rectaLeft(2, 5, 140);
	rectaLeft.SetPos(-37, 2.5f, 40);
	
	Cube rectaRight(2, 5, 120);
	rectaRight.SetPos(-63, 2.5f, 20);
	

	//CORBA ESQUERRA 1
	Cube corbaLeft(2, 5, 20);
	corbaLeft.SetPos(-30, 2.5f, 118);
	corbaLeft.SetRotation(38, ROTATE_Y);

	Cube corbaRight(2, 5, 35);
	corbaRight.SetPos(-57.0f, 2.5f, 97.0f);
	corbaRight.SetRotation(20, ROTATE_Y);

	Cube corbaRight2(2, 5, 40);
	corbaRight2.SetPos(-42, 2.5f, 132);
	corbaRight2.SetRotation(25, ROTATE_Y);


	//RECTA ESQUERRA
	Cube rEsquerraLeft(2, 5, 21);
	rEsquerraLeft.SetPos(-12.5f, 2.5f, 126);
	rEsquerraLeft.SetRotation(90, ROTATE_Y);

	Cube rEsquerraRight(2, 5, 30);
	rEsquerraRight.SetPos(-17.4f, 2.5f, 151);
	rEsquerraRight.SetRotation(90, ROTATE_Y);


	//AMPLIACIO
	Cube ampDown(2, 5, 10);
	ampDown.SetPos(-3, 2.5f, 120);

	Cube ampUp(2, 5, 10);
	ampUp.SetPos(-3, 2.5f, 157);


	//RECTA AMPLIADA
	Cube rectaDown(2, 5, 90);
	rectaDown.SetPos(41, 2.5f, 115);
	rectaDown.SetRotation(90, ROTATE_Y);

	Cube rectaUp(2, 5, 90);
	rectaUp.SetPos(41, 2.5f, 162);
	rectaUp.SetRotation(90, ROTATE_Y);


	//REDUCCIO
	Cube redDown(2, 5, 10);
	redDown.SetPos(87, 2.5f, 120);

	Cube redUp(2, 5, 10);
	redUp.SetPos(87, 2.5f, 157);


	//SORTIDA TRAM AMPLIAT
	Cube tramUp(2, 5, 60);
	tramUp.SetPos(117, 2.5f, 151);
	tramUp.SetRotation(90, ROTATE_Y);

	Cube tramDown(2, 5, 30);
	tramDown.SetPos(102, 2.5f, 126);
	tramDown.SetRotation(90, ROTATE_Y);


	//RECTA CAP ABAIX (amb bots/rampes)
	Cube rAbaixLeft(2, 5, 150);
	rAbaixLeft.SetPos(146, 2.5f, 75);

	Cube rAbaixRight(2, 5, 120);
	rAbaixRight.SetPos(116, 2.5f, 65);


	//DIAGONAL1
	Cube diagonalLeft(2, 5, 50);
	diagonalLeft.SetPos(133, 2.5f, -22);
	diagonalLeft.SetRotation(33, ROTATE_Y);

	Cube diagonalRight(2, 5, 55);
	diagonalRight.SetPos(101, 2.5f, -19);
	diagonalRight.SetRotation(33, ROTATE_Y);


	//DIAGONAL2
	Cube diagonalLeft2(2, 5, 50);
	diagonalLeft2.SetPos(135.4f, 2.5f, -61);
	diagonalLeft2.SetRotation(-45, ROTATE_Y);

	Cube diagonalRight2(2, 5, 55);
	diagonalRight2.SetPos(105, 2.5f, -63);
	diagonalRight2.SetRotation(-45, ROTATE_Y);


	//EMBUT
	Cube coneLeft(2, 5, 35);
	coneLeft.SetPos(150, 2.5f, -97);
	coneLeft.SetRotation(10, ROTATE_Y);

	Cube coneRight(2, 5, 32);
	coneRight.SetPos(127.7f, 2.5f, -99);
	coneRight.SetRotation(-10, ROTATE_Y);


	//CANTONADA ABAIX-ESQUERRA
	Cube cantonadaLeft(2, 5, 15);
	cantonadaLeft.SetPos(147, 2.5f, -122);


	//RECTA CAP A LA DRETA
	Cube rStraightBelow(2, 5, 150);
	rStraightBelow.SetPos(73, 2.5f, -130.4f);
	rStraightBelow.SetRotation(90, ROTATE_Y);

	Cube rStraightAbove(2, 5, 125);
	rStraightAbove.SetPos(69, 2.5f, -116);
	rStraightAbove.SetRotation(90, ROTATE_Y);


	//DIAGONAL ULTIMA
	Cube lastDiagonalLeft(2, 5, 70);
	lastDiagonalLeft.SetPos(-13.5f, 2.5f, -86.2f);
	lastDiagonalLeft.SetRotation(-33, ROTATE_Y);

	Cube lastDiagonalRight(2, 5, 70);
	lastDiagonalRight.SetPos(-24.5f, 2.5f, -102.5f);
	lastDiagonalRight.SetRotation(-38, ROTATE_Y);


	//DIAGONAL ULTIMA PART 2
	Cube sobraLeft(2, 5, 27);
	sobraLeft.SetPos(-35, 2.5f, -43);
	sobraLeft.SetRotation(-7, ROTATE_Y);

	Cube sobraRight(2, 5, 37);
	sobraRight.SetPos(-54.8f, 2.5f, -57.3f);
	sobraRight.SetRotation(-25, ROTATE_Y);

	rectaLeft.Render();
	rectaRight.Render();
	corbaLeft.Render();
	corbaRight.Render();
	corbaRight2.Render();
	rEsquerraLeft.Render();
	rEsquerraRight.Render();
	ampDown.Render();
	ampUp.Render();
	rectaDown.Render();
	rectaUp.Render();
	redDown.Render();
	redUp.Render();
	tramUp.Render();
	tramDown.Render();
	rAbaixLeft.Render();
	rAbaixRight.Render();
	diagonalLeft.Render();
	diagonalRight.Render();
	diagonalLeft2.Render();
	diagonalRight2.Render();
	coneLeft.Render();
	coneRight.Render();
	cantonadaLeft.Render();
	rStraightBelow.Render();
	rStraightAbove.Render();
	lastDiagonalLeft.Render();
	lastDiagonalRight.Render();
	sobraLeft.Render();
	sobraRight.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::CreateCircuit()
{

	int i = 0;
	
	//RECTA PRINCIPAL
	Cube rectaLeft(2, 5, 140);
	rectaLeft.SetPos(-37, 1, 40);
	App->physics->AddBody(rectaLeft,100000);
	Cube rectaRight(2, 5, 120);
	rectaRight.SetPos(-63, 1, 20);
	App->physics->AddBody(rectaRight,100000);

	//CORBA ESQUERRA 1
	Cube corbaLeft(2, 5, 20);
	corbaLeft.SetPos(-30, 1, 118);
	corbaLeft.SetRotation(38, ROTATE_Y);
	App->physics->AddBody(corbaLeft, 100000);
	Cube corbaRight(2, 5, 35);
	corbaRight.SetPos(-57.0f, 1, 97.0f);
	corbaRight.SetRotation(20, ROTATE_Y);
	App->physics->AddBody(corbaRight, 100000);
	Cube corbaRight2(2, 5, 40);
	corbaRight2.SetPos(-42, 1, 132);
	corbaRight2.SetRotation(25, ROTATE_Y);
	App->physics->AddBody(corbaRight2, 100000);

	//RECTA ESQUERRA
	Cube rEsquerraLeft(2, 5, 21);
	rEsquerraLeft.SetPos(-12.5f, 1, 126);
	rEsquerraLeft.SetRotation(90, ROTATE_Y);
	App->physics->AddBody(rEsquerraLeft, 100000);
	Cube rEsquerraRight(2, 5, 30);
	rEsquerraRight.SetPos(-17.4f, 1, 151);
	rEsquerraRight.SetRotation(90, ROTATE_Y);
	App->physics->AddBody(rEsquerraRight, 100000);

	//AMPLIACIO
	Cube ampDown(2, 5, 10);
	ampDown.SetPos(-3, 1, 120);
	App->physics->AddBody(ampDown, 100000);
	Cube ampUp(2, 5, 10);
	ampUp.SetPos(-3, 1, 157);
	App->physics->AddBody(ampUp, 100000);

	//RECTA AMPLIADA
	Cube rectaDown(2, 5, 90);
	rectaDown.SetPos(41, 1, 115);
	rectaDown.SetRotation(90, ROTATE_Y);
	App->physics->AddBody(rectaDown, 100000);
	Cube rectaUp(2, 5, 90);
	rectaUp.SetPos(41, 1, 162);
	rectaUp.SetRotation(90, ROTATE_Y);
	App->physics->AddBody(rectaUp, 100000);

	//REDUCCIO
	Cube redDown(2, 5, 10);
	redDown.SetPos(87, 1, 120);
	App->physics->AddBody(redDown, 100000);
	Cube redUp(2, 5, 10);
	redUp.SetPos(87, 1, 157);
	App->physics->AddBody(redUp, 100000);

	//SORTIDA TRAM AMPLIAT
	Cube tramUp(2, 5, 60);
	tramUp.SetPos(117, 1, 151);
	tramUp.SetRotation(90, ROTATE_Y);
	App->physics->AddBody(tramUp, 100000);
	Cube tramDown(2, 5, 30);
	tramDown.SetPos(102, 1, 126);
	tramDown.SetRotation(90, ROTATE_Y);
	App->physics->AddBody(tramDown, 100000);

	//RECTA CAP ABAIX (amb bots/rampes)
	Cube rAbaixLeft(2, 5, 150);
	rAbaixLeft.SetPos(146, 1, 75);
	App->physics->AddBody(rAbaixLeft, 100000);
	Cube rAbaixRight(2, 5, 120);
	rAbaixRight.SetPos(116, 1, 65);
	App->physics->AddBody(rAbaixRight, 100000);

	//DIAGONAL1
	Cube diagonalLeft(2, 5, 50);
	diagonalLeft.SetPos(133, 1, -22);
	diagonalLeft.SetRotation(33, ROTATE_Y);
	App->physics->AddBody(diagonalLeft, 100000);
	Cube diagonalRight(2, 5, 55);
	diagonalRight.SetPos(101, 1, -19);
	diagonalRight.SetRotation(33, ROTATE_Y);
	App->physics->AddBody(diagonalRight, 100000);

	//DIAGONAL2
	Cube diagonalLeft2(2, 5, 50);
	diagonalLeft2.SetPos(135.4f, 1, -61);
	diagonalLeft2.SetRotation(-45, ROTATE_Y);
	App->physics->AddBody(diagonalLeft2, 100000);
	Cube diagonalRight2(2, 5, 55);
	diagonalRight2.SetPos(105, 1, -63);
	diagonalRight2.SetRotation(-45, ROTATE_Y);
	App->physics->AddBody(diagonalRight2, 100000);

	//EMBUT
	Cube coneLeft(2, 5, 35);
	coneLeft.SetPos(150, 1, -97);
	coneLeft.SetRotation(10, ROTATE_Y);
	App->physics->AddBody(coneLeft, 100000);
	Cube coneRight(2, 5, 32);
	coneRight.SetPos(127.7f, 1, -99);
	coneRight.SetRotation(-10, ROTATE_Y);
	App->physics->AddBody(coneRight, 100000);

	//CANTONADA ABAIX-ESQUERRA
	Cube cantonadaLeft(2, 5, 15);
	cantonadaLeft.SetPos(147, 1, -122);
	App->physics->AddBody(cantonadaLeft, 100000);

	//RECTA CAP A LA DRETA
	Cube rStraightBelow(2, 5, 150);
	rStraightBelow.SetPos(73, 1, -130.4f);
	rStraightBelow.SetRotation(90, ROTATE_Y);
	App->physics->AddBody(rStraightBelow, 100000);
	Cube rStraightAbove(2, 5, 125);
	rStraightAbove.SetPos(69, 1, -116);
	rStraightAbove.SetRotation(90, ROTATE_Y);
	App->physics->AddBody(rStraightAbove, 10000);

	//DIAGONAL ULTIMA
	Cube lastDiagonalLeft(2, 5, 70);
	lastDiagonalLeft.SetPos(-13.5f, 1, -86.2f);
	lastDiagonalLeft.SetRotation(-33, ROTATE_Y);
	App->physics->AddBody(lastDiagonalLeft, 100000);
	Cube lastDiagonalRight(2, 5, 70);
	lastDiagonalRight.SetPos(-24.5f, 1, -102.5f);
	lastDiagonalRight.SetRotation(-38, ROTATE_Y);
	App->physics->AddBody(lastDiagonalRight, 100000);

	//DIAGONAL ULTIMA PART 2
	Cube sobraLeft(2, 5, 27);
	sobraLeft.SetPos(-35, 1, -43);
	sobraLeft.SetRotation(-7, ROTATE_Y);
	App->physics->AddBody(sobraLeft, 100000);
	Cube sobraRight(2, 5, 37);
	sobraRight.SetPos(-54.8f, 1, -57.3f);
	sobraRight.SetRotation(-25, ROTATE_Y);
	App->physics->AddBody(sobraRight, 100000);

	/*if (render == false)
	{
		render = !render;
		while (render)
		{
			rectaLeft.Render();
			rectaRight.Render();
			corbaLeft.Render();
			corbaRight.Render();
			corbaRight2.Render();
			rEsquerraLeft.Render();
			rEsquerraRight.Render();
			ampDown.Render();
			ampUp.Render();
			rectaDown.Render();
			rectaUp.Render();
			redDown.Render();
			redUp.Render();
			tramUp.Render();
			tramDown.Render();
			rAbaixLeft.Render();
			rAbaixRight.Render();
			diagonalLeft.Render();
			diagonalRight.Render();
			diagonalLeft2.Render();
			diagonalRight2.Render();
			coneLeft.Render();
			coneRight.Render();
			cantonadaLeft.Render();
			rStraightBelow.Render();
			rStraightAbove.Render();
			lastDiagonalLeft.Render();
			lastDiagonalRight.Render();
			sobraLeft.Render();
			sobraRight.Render();
			if (i == 0)
			{
				App->physics->AddBody(rectaLeft, 100000);
				App->physics->AddBody(rectaRight, 100000);
				App->physics->AddBody(corbaLeft, 100000);
				App->physics->AddBody(corbaRight, 100000);
				App->physics->AddBody(corbaRight2, 100000);
				App->physics->AddBody(rEsquerraLeft, 100000);
				App->physics->AddBody(rEsquerraRight, 100000);
				App->physics->AddBody(ampDown, 100000);
				App->physics->AddBody(ampUp, 100000);
				App->physics->AddBody(rectaDown, 100000);
				App->physics->AddBody(rectaUp, 100000);
				App->physics->AddBody(redDown, 100000);
				App->physics->AddBody(redUp, 100000);
				App->physics->AddBody(tramUp, 100000);
				App->physics->AddBody(tramDown, 100000);
				App->physics->AddBody(rAbaixLeft, 100000);
				App->physics->AddBody(rAbaixRight, 100000);
				App->physics->AddBody(diagonalLeft, 100000);
				App->physics->AddBody(diagonalRight, 100000);
				App->physics->AddBody(diagonalLeft2, 100000);
				App->physics->AddBody(diagonalRight2, 100000);
				App->physics->AddBody(coneLeft, 100000);
				App->physics->AddBody(coneRight, 100000);
				App->physics->AddBody(cantonadaLeft, 100000);
				App->physics->AddBody(rStraightBelow, 100000);
				App->physics->AddBody(rStraightAbove, 10000);
				App->physics->AddBody(lastDiagonalLeft, 100000);
				App->physics->AddBody(sobraLeft, 100000);
				App->physics->AddBody(lastDiagonalRight, 100000);
				App->physics->AddBody(sobraRight, 100000);

				i += 1;
			}
		}
		

	}*/

}

