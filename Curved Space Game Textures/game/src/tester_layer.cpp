#include "tester_layer.h"
#include "game_world.h"
#include "platform/opengl/gl_shader.h"

tester_layer::tester_layer(engine::ref<game_world> game_world, std::vector<engine::layer*> layers)
	:m_game_world(game_world), m_layers(layers)
{
	myfile.open("test.csv");
	myfile << "Approach name, Curvature (K), Tessellation Parameter, Number of Objects, Average FPS\n";
}

tester_layer::~tester_layer()
{
	m_game_world->set_num_objects(100);
}

void tester_layer::on_update(const engine::timestep& time_step)
{
	if (m_game_world->testing_mode())
	{
		m_test_timer += (float)time_step;
			//test if current test finished
		

		if (m_test_timer > m_timer_end)
		{
			//if (m_game_world->num_objects() > 0)
			if (m_game_world->tess_parameter() > 15)
			{
				// average out the fps value dividing by the number of entries. 
				double fps_average = m_fps_sum / count_sum_instances;

				// Write to log file
				if (current_cycle == 0)
				{
					/**
					std::string extra_info = "";
					if ((m_game_world->active_layer_index() == 1 || m_game_world->active_layer_index() == 3) && m_game_world->get_K() > 0.f)
					{
						if (m_game_world->great_circle_navigation())
							extra_info = "; great circle navigation";
						else
							extra_info = "; cartesian vectors";
					}
					if ((m_game_world->active_layer_index() == 0 || m_game_world->active_layer_index() == 2) && m_game_world->get_K() == 0.f)
					{
						if (m_game_world->non_curved_tesselation())
							extra_info = "; non_curved_tesselation";
					}*/
					m_record_info.push_back(m_game_world->layer_names().at(m_game_world->active_layer_index()) + "," + std::to_string(m_game_world->get_K()) + "," + std::to_string(m_game_world->tess_parameter()) + "," + std::to_string(num_obj) + ",");

					m_fps_average_sum.push_back(fps_average);
				}
				else
				{
					m_fps_average_sum.at(record_index) += fps_average;
				}

				m_fps_sum = 0.0;
				count_sum_instances = 0;

				//  switch to next test
				record_index++;
			}

			
			next_test();

			m_test_timer = 0.f;
		}
		else
		{
			//if (m_game_world->num_objects() > 0)
			if (m_game_world->tess_parameter() > 0)
			{
				//  continue running the test by adding the fps count to the sum and increasing the number of instances
				m_fps_sum += m_game_world->fps();
				count_sum_instances++;
			}
		}
		
	}
}

void tester_layer::on_render()
{
	//nothing

}

void tester_layer::on_event(engine::event& event)
{
	//nothing
}

void tester_layer::next_test()
{
	/*increment_counter++;
	if (increment_counter == 10)
	{
		increment = increment * 2;
		increment_counter = 0;
	}*/
	//float next_increment = ceil(pow((increment * 0.1f), 2));

	// test whether over the limit of number of objects
	//if (m_game_world->num_objects() + next_increment >= max_tessellation)
	//if (m_game_world->tess_parameter() + next_increment >= max_tessellation)
	if (increment >= max_tessellation)
	{

		// switch the algorithm
		//m_game_world->set_num_objects(0);
		increment = 15;
		m_game_world->set_tess_parameter(increment);
		m_game_world->set_num_objects(0);
		

		//if (m_game_world->get_K() == 0.f)
		//	m_game_world->set_K(-1.f);
		//else if (m_game_world->get_K() < 0.f)
		//	m_game_world->set_K(1.f);
		//else
		{
			int active_layer_index = m_game_world->active_layer_index();
			/*
			if (active_layer_index == 1 || active_layer_index == 3)
			{
				if (!m_game_world->great_circle_navigation())
				{
					m_game_world->set_great_circle_navigation(true);
				}
				else
				{
					m_game_world->set_great_circle_navigation(false);
				}
			}
			else {
				if (!m_game_world->non_curved_tesselation())
				{
					m_game_world->set_non_curved_tesselation(true);
					m_game_world->set_K(0.f);
				}
				else
				{
					m_game_world->set_non_curved_tesselation(false);
				}
			}
			*/

			//if (!m_game_world->great_circle_navigation() && !m_game_world->non_curved_tesselation())
			//{
			m_game_world->set_K(0.f);
			//if (active_layer_index == 1)
			if(num_obj < 800)
			{
				num_obj *= 2;
				//m_layers.at(active_layer_index)->set_active(false);
				//int new_layer_index = 2;
				//m_game_world->set_active_layer_index(new_layer_index);
				//m_layers.at(new_layer_index)->set_active(true);
			}
			else
			{
				if (current_cycle < num_cycles)
				{
					//m_layers.at(active_layer_index)->set_active(false);
					//m_game_world->set_active_layer_index(1);
					//m_layers.at(2)->set_active(true);
					current_cycle++;
					record_index = 0;
					num_obj = 100;
				}
				else
				{
					write_to_log_file();
					myfile.close();
					engine::application::exit();
				}
			}
			//}
		}
	}
	else
	{
		// increase number of objects
		//m_game_world->set_num_objects(m_game_world->num_objects() + next_increment);
		//m_game_world->set_tess_parameter(m_game_world->tess_parameter() + next_increment);
		if (increment == 15)
			m_game_world->set_num_objects(num_obj);

		increment = increment + 5;

		//if(m_game_world->active_layer_index() == 1)
		m_game_world->set_tess_parameter(increment);
		//else
		//	m_game_world->set_tess_parameter(sqrt(increment));
		
		
		
	}
}

void tester_layer::write_to_log_file()
{
	for (int i = 0; i < m_record_info.size(); i++) {
		myfile << m_record_info.at(i) << std::to_string((int) (m_fps_average_sum.at(i) / (num_cycles+1))) << "\n";
	}
	
	/*std::ofstream log_file(
		"log_file.txt", std::ios_base::out | std::ios_base::app);
	log_file << text << std::endl;*/
}
