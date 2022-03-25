#type vertex
#version 330 core
			
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;
		layout(location = 5) in int a_Index;		

		uniform mat4 u_ViewProjection;

		out vec4 v_Color;
		flat out int v_Index;

		void main()
		{
			v_Color = a_Color;
			v_Index = a_Index;
			
			gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
		}

#type fragment
#version 330 core
			
		layout(location = 0) out vec4 color;
		layout(location = 1) out int color2;

		in vec4 v_Color;
		flat in int v_Index;


		void main()
		{
			
			color = v_Color;
						

			color2 = v_Index;
		}