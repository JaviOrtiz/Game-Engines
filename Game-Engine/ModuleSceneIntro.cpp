#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
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
	if (timepassed >= 60000 && youlost == false) {
		lost = true;
		Lost();
		youlost = true;
	}

	if (started == false) {
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			time = SDL_GetTicks();
			started = true;
			start.color = White;
			door1l.color = Red;
			door1r.color = Red;
		}
	}
	if (started == true && finished == false) {
		timepassed = SDL_GetTicks() - time;
	}
	
	if (finished == true && finished2 == false) {
		finished2 = true;
		App->audio->CleanUp();
		App->audio->Init ();
		victory = App->audio->LoadFx("Victory.wav");
		App->audio->PlayFx(victory);
	}

	if (check1 == true && checks1 == false) {
		App->audio->PlayFx(checkpoint);
		checks1 = true;
	}
	if (check2 == true && checks2 == false) {
		App->audio->PlayFx(checkpoint);
		checks2 = true;
	}
	if (check3 == true && checks3 == false) {
		App->audio->PlayFx(checkpoint);
		checks3 = true;
	}
	if (check4 == true && checks4 == false) {
		App->audio->PlayFx(checkpoint);
		checks4 = true;
	}
	if (check5 == true && checks5 == false) {
		App->audio->PlayFx(checkpoint);
		checks5 = true;
	}
	if (check6 == true && checks6 == false) {
		App->audio->PlayFx(checkpoint);
		checks6 = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		Restart();
	}

	Flooor.Render();
	door6_1.Render();
	door6_2.Render();
	door6_3.Render();
	door6_4.Render();
	start.Render();
	door1l.Render();
	door1r.Render();
	door2palo.Render();
	door5palo.Render();
	door4_1.Render();
	door4_2.Render();
	door4_3.Render();
	door7_1.Render();
	door7_2.Render();
	door7_3.Render();
	end.Render();

	/////////Ring Render/////////
	ring1.Render();
	ring2.Render();
	ring3.Render();
	ring4.Render();
	ring5.Render();
	ring6.Render();
	ring7.Render();
	ring8.Render();
	ring9.Render();
	ring10.Render();
	ring11.Render();
	ring12.Render();


	/////////Ring 2 Render /////////
	ring13.Render();
	ring14.Render();
	ring15.Render();
	ring16.Render();
	ring17.Render();
	ring18.Render();
	ring19.Render();
	ring20.Render();
	ring21.Render();
	ring22.Render();
	ring23.Render();
	ring24.Render();

	////////Ring Colour //////////

	


	char title[80];
	sprintf_s(title, "%.3f Seconds", timepassed/1000);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1 == Door1 || body2 == Door1) {
		if (youlost == false) {
			door2palo.color = Red;
			door1l.color = White;
			door1r.color = White;
			ring1.color.Set(255, 255, 0, 1);
			ring2.color.Set(255, 255, 0, 1);
			ring3.color.Set(255, 255, 0, 1);
			ring4.color.Set(255, 255, 0, 1);
			ring5.color.Set(255, 255, 0, 1);
			ring6.color.Set(255, 255, 0, 1);
			ring7.color.Set(255, 255, 0, 1);
			ring8.color.Set(255, 255, 0, 1);
			ring9.color.Set(255, 255, 0, 1);
			ring10.color.Set(255, 255, 0, 1);
			ring11.color.Set(255, 255, 0, 1);
			ring12.color.Set(255, 255, 0, 1);
			check1 = true;
		}
	}
	if (body1 == Door2 || body2 == Door2) {
		if (check1 == true && youlost == false) {
			door2palo.color = White;
			ring1.color = White;
			ring2.color = White;
			ring3.color = White;
			ring4.color = White;
			ring5.color = White;
			ring6.color = White;
			ring7.color = White;
			ring8.color = White;
			ring9.color = White;
			ring10.color = White;
			ring11.color = White;
			ring12.color = White;
			door4_1.color = Blue;
			door4_2.color = Blue;
			door4_3.color = Blue;
			check2 = true;
		}
	}
	if (body1 == Door3 || body2 == Door3) {
		if (check2 == true && youlost == false) {
			door4_1.color = White;
			door4_2.color = White;
			door4_3.color = White;
			ring13.color.Set(255, 255, 0, 1);
			ring14.color.Set(255, 255, 0, 1);
			ring15.color.Set(255, 255, 0, 1);
			ring16.color.Set(255, 255, 0, 1);
			ring17.color.Set(255, 255, 0, 1);
			ring18.color.Set(255, 255, 0, 1);
			ring19.color.Set(255, 255, 0, 1);
			ring20.color.Set(255, 255, 0, 1);
			ring21.color.Set(255, 255, 0, 1);
			ring22.color.Set(255, 255, 0, 1);
			ring23.color.Set(255, 255, 0, 1);
			ring24.color.Set(255, 255, 0, 1);
			door5palo.color = Red;
			check3 = true;
		}
	}
	if (body1 == Door4 || body2 == Door4) {
		if (check3 == true && youlost == false) {
			ring13.color = White;
			ring14.color = White;
			ring15.color = White;
			ring16.color = White;
			ring17.color = White;
			ring18.color = White;
			ring19.color = White;
			ring20.color = White;
			ring21.color = White;
			ring22.color = White;
			ring23.color = White;
			ring24.color = White;
			door5palo.color = White;

			door6_1.color = Red;
			door6_2.color = Red;
			door6_3.color = Red;
			door6_4.color = Red;
			check4 = true;
		}
	}

	if (body1 == Door5 || body2 == Door5) {
		if (check4 == true && youlost == false) {
			door6_1.color = White;
			door6_2.color = White;
			door6_3.color = White;
			door6_4.color = White;
			door7_1.color = Blue;
			door7_2.color = Blue;
			door7_3.color = Blue;
			check5 = true;
		}
	}
	if (body1 == Door6 || body2 == Door6) {
		if (check5 == true && youlost == false) {
			door7_1.color = White;
			door7_2.color = White;
			door7_3.color = White;
			end.color = Green;
			check6 = true;
		}
	}
	if (body1 == Door7 || body2 == Door7) {
		if (check6 == true && youlost == false) {
			end.color = White;
			finished = true;
		}
	}
	else LOG("Hit!");
}

void ModuleSceneIntro::Restart() {
	youlost = false;
	timepassed = 0;
	lost = false;
	started = false;
	finished = false;
	finished2 = false;
	check1 = false;
	check2 = false;
	check3 = false;
	check4 = false;
	check5 = false;
	check6 = false;
	checks1 = false;
	checks2 = false;
	checks3 = false;
	checks4 = false;
	checks5 = false;
	checks6 = false;
	door1l.color = White;
	door1r.color = White;
	start.color = Green;

	door2palo.color = White;
	ring1.color = White;
	ring2.color = White;
	ring3.color = White;
	ring4.color = White;
	ring5.color = White;
	ring6.color = White;
	ring7.color = White;
	ring8.color = White;
	ring9.color = White;
	ring10.color = White;
	ring11.color = White;
	ring12.color = White;
	door4_1.color = White;
	door4_2.color = White;
	door4_3.color = White;
	ring13.color = White;
	ring14.color = White;
	ring15.color = White;
	ring16.color = White;
	ring17.color = White;
	ring18.color = White;
	ring19.color = White;
	ring20.color = White;
	ring21.color = White;
	ring22.color = White;
	ring23.color = White;
	ring24.color = White;
	door5palo.color = White;
	door6_1.color = White;
	door6_2.color = White;
	door6_3.color = White;
	door6_4.color = White;
	door7_1.color = White;
	door7_2.color = White;
	door7_3.color = White;
	end.color = White;

	App->audio->CleanUp();
	App->audio->Init();
	victory = App->audio->LoadFx("Victory.wav");
	musicc = App->audio->LoadFx("Undertale_-_Bonetrousle_1_1_1_1_.wav");
	wasted = App->audio->LoadFx("Wasted.wav");
	checkpoint = App->audio->LoadFx("Sonic_Ring_Sound.wav");
	App->audio->PlayFx(musicc, -1);
}


void ModuleSceneIntro::CreateMap() {
	start.radius = 7;
	start.height = 1;
	start.SetRotation(120, (0, 0, 2));
	start.SetPos(0, 1, 0);
	start.color = Green;
	map[0] = App->physics->AddBody(start, 0.0f);


	door1l.radius = 0.5f;
	door1l.height = 20;
	door1l.SetRotation(120, (0, 0, 1));
	door1l.SetPos(30, 10, -5);
	door1l.color = White;
	map[1] = App->physics->AddBody(door1l, 0.0f);

	door1r.radius = 0.5f;
	door1r.height = 20;
	door1r.SetRotation(120, (0, 0, 1));
	door1r.SetPos(30, 10, 5);
	door1r.color = White;
	map[2] = App->physics->AddBody(door1r, 0.0f);

	door2palo.radius = 0.5f;
	door2palo.height = 20;
	door2palo.SetRotation(120, (0, 0, 1));
	door2palo.SetPos(70, 10, 0);
	door2palo.color = White;
	map[3] = App->physics->AddBody(door2palo, 0.0f);

	door5palo.radius = 0.5f;
	door5palo.height = 10;
	door5palo.SetRotation(120, (0, 0, 1));
	door5palo.SetPos(40, 5, -40);
	door5palo.color = White;
	map[27] = App->physics->AddBody(door5palo, 0.0f);

	door4_1.radius = 0.5f;
	door4_1.height = 8;
	door4_1.SetRotation(120, (0, 0, 1));
	door4_1.SetPos(0, 4, -30);
	door4_1.color = White;
	map[4] = App->physics->AddBody(door4_1, 0.0f);

	door4_2.radius = 0.5f;
	door4_2.height = 8;
	door4_2.SetRotation(120, (0, 0, 1));
	door4_2.SetPos(8, 4, -30);
	door4_2.color = White;
	map[5] = App->physics->AddBody(door4_2, 0.0f);

	door4_3.radius = 0.5f;
	door4_3.height = 8;
	door4_3.SetPos(4, 7, -30);
	door4_3.color = White;
	map[6] = App->physics->AddBody(door4_3, 0.0f);

	door7_1.radius = 0.5f;
	door7_1.height = 8;
	door7_1.SetRotation(120, (0, 0, 1));
	door7_1.SetPos(70, 4, -30);
	door7_1.color = White;
	map[7] = App->physics->AddBody(door7_1, 0.0f);

	door7_2.radius = 0.5f;
	door7_2.height = 8;
	door7_2.SetRotation(120, (0, 0, 1));
	door7_2.SetPos(78, 4, -30);
	door7_2.color = White;
	map[8] = App->physics->AddBody(door7_2, 0.0f);

	door7_3.radius = 0.5f;
	door7_3.height = 8;
	door7_3.SetPos(74, 7, -30);
	door7_3.color = White;
	map[9] = App->physics->AddBody(door7_3, 0.0f);

	s.size = vec3(5, 3, 1);
	s.SetPos(0, 2.5f, 20);

	door6_1.size = vec3(12, 7, 1);
	door6_1.SetPos(50, 4, -20);
	map[10] = App->physics->AddBody(door6_1, 0.0f);

	door6_2.size = vec3(2, 10, 1);
	door6_2.SetPos(55, 9, -20);
	map[11] = App->physics->AddBody(door6_2, 0.0f);

	door6_3.size = vec3(2, 10, 1);
	door6_3.SetPos(45, 9, -20);
	map[12] = App->physics->AddBody(door6_3, 0.0f);

	door6_4.size = vec3(10, 2, 1);
	door6_4.SetPos(50, 13, -20);
	map[13] = App->physics->AddBody(door6_4, 0.0f);


	///////////////////RING 1 ///////////////

	vec3 Ringbox(1, 1, 2);

	////radius////
	ring1.size = Ringbox;
	ring2.size = Ringbox;
	ring3.size = Ringbox;
	ring4.size = Ringbox;
	ring5.size = Ringbox;
	ring6.size = Ringbox;
	ring7.size = Ringbox;
	ring8.size = Ringbox;
	ring9.size = Ringbox;
	ring10.size = Ringbox;
	ring11.size = Ringbox;
	ring12.size = Ringbox;



	////Position////


	ring1.SetPos(70, 20, 0);
	ring2.SetPos(70, 20.6f, -2);
	ring2.SetRotation(36, { 1,0,0 });
	ring3.SetPos(70, 22.4f, -3.4f);
	ring3.SetRotation(72, { 1,0,0 });
	ring4.SetPos(70, 24.5f, -3.8f);
	ring4.SetRotation(90, { 1,0,0 });
	ring5.SetPos(70, 26.6, -3.4f);
	ring5.SetRotation(116, { 1,0,0 });
	ring6.SetPos(70, 28.2f, -2);
	ring6.SetRotation(152, { 1,0,0 });
	ring7.SetPos(70, 28.7, -0);
	ring7.SetRotation(0, { 1,0,0 });
	ring8.SetPos(70, 28.2, 2);
	ring8.SetRotation(-152, { 1,0,0 });
	ring9.SetPos(70, 26.6, 3.4f);
	ring9.SetRotation(-116, { 1,0,0 });
	ring10.SetPos(70, 24.5f, 3.8f);
	ring10.SetRotation(90, { 1,0,0 });
	ring11.SetPos(70, 22.4f, 3.4f);
	ring11.SetRotation(-72, { 1,0,0 });
	ring12.SetPos(70, 20.6f, 2);
	ring12.SetRotation(-36, { 1,0,0 });

	//////ADD RING/////
	map[15] = App->physics->AddBody(ring1, 0.0f);
	map[16] = App->physics->AddBody(ring2, 0.0f);
	map[17] = App->physics->AddBody(ring3, 0.0f);
	map[18] = App->physics->AddBody(ring4, 0.0f);
	map[19] = App->physics->AddBody(ring5, 0.0f);
	map[20] = App->physics->AddBody(ring6, 0.0f);
	map[21] = App->physics->AddBody(ring7, 0.0f);
	map[22] = App->physics->AddBody(ring8, 0.0f);
	map[23] = App->physics->AddBody(ring9, 0.0f);
	map[24] = App->physics->AddBody(ring10, 0.0f);
	map[25] = App->physics->AddBody(ring11, 0.0f);
	map[26] = App->physics->AddBody(ring12, 0.0f);


	////////Ring 2////////
	ring13.size = Ringbox;
	ring14.size = Ringbox;
	ring15.size = Ringbox;
	ring16.size = Ringbox;
	ring17.size = Ringbox;
	ring18.size = Ringbox;
	ring19.size = Ringbox;
	ring20.size = Ringbox;
	ring21.size = Ringbox;
	ring22.size = Ringbox;
	ring23.size = Ringbox;
	ring24.size = Ringbox;



	////Position////

	//(70, 10, 0);
	//(40, 5, -40);

	ring13.SetPos(40, 10, -40);
	ring14.SetPos(40, 10.6f, -42);
	ring14.SetRotation(36, { 1,0,0 });
	ring15.SetPos(40, 12.4f, -43.4f);
	ring15.SetRotation(72, { 1,0,0 });
	ring16.SetPos(40, 14.5f, -43.8f);
	ring16.SetRotation(90, { 1,0,0 });
	ring17.SetPos(40, 16.6, -43.4f);
	ring17.SetRotation(116, { 1,0,0 });
	ring18.SetPos(40, 18.2f, -42);
	ring18.SetRotation(152, { 1,0,0 });
	ring19.SetPos(40, 18.7, -40);
	ring19.SetRotation(0, { 1,0,0 });
	ring20.SetPos(40, 18.2, -38);
	ring20.SetRotation(-152, { 1,0,0 });
	ring21.SetPos(40, 16.6, -36.4f);
	ring21.SetRotation(-116, { 1,0,0 });
	ring22.SetPos(40, 14.5f, -36.0f);
	ring22.SetRotation(90, { 1,0,0 });
	ring23.SetPos(40, 12.4f, -36.4f);
	ring23.SetRotation(-72, { 1,0,0 });
	ring24.SetPos(40, 10.6f, -38);
	ring24.SetRotation(-36, { 1,0,0 });

	//////ADD RING/////
	map[28] = App->physics->AddBody(ring13, 0.0f);
	map[29] = App->physics->AddBody(ring14, 0.0f);
	map[30] = App->physics->AddBody(ring15, 0.0f);
	map[31] = App->physics->AddBody(ring16, 0.0f);
	map[32] = App->physics->AddBody(ring17, 0.0f);
	map[33] = App->physics->AddBody(ring18, 0.0f);
	map[34] = App->physics->AddBody(ring19, 0.0f);
	map[35] = App->physics->AddBody(ring20, 0.0f);
	map[36] = App->physics->AddBody(ring21, 0.0f);
	map[37] = App->physics->AddBody(ring22, 0.0f);
	map[38] = App->physics->AddBody(ring23, 0.0f);
	map[39] = App->physics->AddBody(ring24, 0.0f);

	end.radius = 7;
	end.height = 1;
	end.SetRotation(120, (0, 0, 2));
	end.SetPos(60, 1, -60);
	end.color = White;
	map[40] = App->physics->AddBody(end, 0.0f);


	Flooor.size = vec3(100, 0, 80);
	Flooor.SetPos(40, 0.1f, -30);
	map[14] = App->physics->AddBody(Flooor, 0.0f);



	/////////////////////////SENSORS////////////////////////////

	sensor1.size = vec3(0.1f, 20, 9);
	sensor1.SetPos(30, 10, 0);
	Door1 = App->physics->AddBody(sensor1, 0.0f);
	Door1->SetAsSensor(true);
	Door1->collision_listeners.add(this);

	sensor2.height = 0.1f;
	sensor2.radius = 4;
	sensor2.SetPos(70, 24, 0);
	Door2 = App->physics->AddBody(sensor2, 0.0f);
	Door2->SetAsSensor(true);
	Door2->collision_listeners.add(this);

	sensor3.size = vec3(8, 7, 0.1f);
	sensor3.SetPos(4, 3, -30);
	Door3 = App->physics->AddBody(sensor3, 0.0f);
	Door3->SetAsSensor(true);
	Door3->collision_listeners.add(this);

	sensor4.height = 0.1f;
	sensor4.radius = 4;
	sensor4.SetPos(40, 14, -40);
	Door4 = App->physics->AddBody(sensor4, 0.0f);
	Door4->SetAsSensor(true);
	Door4->collision_listeners.add(this);


	sensor5.size = vec3(8, 6, 0.1f);
	sensor5.SetPos(50, 10, -20);
	sensor5.color = Red;
	Door5 = App->physics->AddBody(sensor5, 0.0f);
	Door5->SetAsSensor(true);
	Door5->collision_listeners.add(this);


	sensor6.size = vec3(8, 7, 0.1f);
	sensor6.SetPos(74, 3, -30);
	Door6 = App->physics->AddBody(sensor6, 0.0f);
	Door6->SetAsSensor(true);
	Door6->collision_listeners.add(this);

	sensor7.radius = 7;
	sensor7.height = 0.1f;
	sensor7.SetRotation(120, (0, 0, 2));
	sensor7.SetPos(60, 1.5f, -60);
	Door7 = App->physics->AddBody(sensor7, 0.0f);
	Door7->SetAsSensor(true);
	Door7->collision_listeners.add(this);

}

void ModuleSceneIntro::Lost() {
	App->audio->CleanUp();
	App->audio->Init();
	victory = App->audio->LoadFx("Victory.wav");
	musicc = App->audio->LoadFx("Undertale_-_Bonetrousle_1_1_1_1_.wav");
	wasted = App->audio->LoadFx("Wasted.wav");
	checkpoint = App->audio->LoadFx("Sonic_Ring_Sound.wav");
	App->audio->PlayFx(wasted);
	timepassed = 60000;
	door1l.color = White;
	door1r.color = White;
	door2palo.color = White;
	ring1.color = White;
	ring2.color = White;
	ring3.color = White;
	ring4.color = White;
	ring5.color = White;
	ring6.color = White;
	ring7.color = White;
	ring8.color = White;
	ring9.color = White;
	ring10.color = White;
	ring11.color = White;
	ring12.color = White;
	door4_1.color = White;
	door4_2.color = White;
	door4_3.color = White;
	ring13.color = White;
	ring14.color = White;
	ring15.color = White;
	ring16.color = White;
	ring17.color = White;
	ring18.color = White;
	ring19.color = White;
	ring20.color = White;
	ring21.color = White;
	ring22.color = White;
	ring23.color = White;
	ring24.color = White;
	door5palo.color = White;
	door6_1.color = White;
	door6_2.color = White;
	door6_3.color = White;
	door6_4.color = White;
	door7_1.color = White;
	door7_2.color = White;
	door7_3.color = White;
	end.color = White;
}