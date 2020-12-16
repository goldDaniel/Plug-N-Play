#include "SpriteBatch.h"

    
/**
 * @brief Resets the buffers, this will cause additional calls to 
 * overwrite the data currently held.
 * 
 */
void SpriteBatch::Clear()
{
    memset(&position_buffer[0], 0, POS_BUFFER_SIZE * sizeof(float));
    memset(&color_buffer[0], 0, COL_BUFFER_SIZE * sizeof(float));
    memset(&texcoord_buffer[0], 0, TEX_BUFFER_SIZE * sizeof(float));

    memset(&index_buffer[0], 0, IDX_BUFFER_SIZE * sizeof(unsigned int));

    current_pos_index = 0;
    current_col_index = 0;
    current_tex_index = 0;
    current_idx_index = 0;

    current_vertex_index = 0;

    current_texture = nullptr;
}

/**
 * @brief Copys the data held in our buffers to the GPU, and then renders to the screen.
 * 
 */
void SpriteBatch::Flush()
{
    if(current_texture == nullptr) return;

    assert(has_begun && "ShapeRenderer must be running to flush data to GPU");

    glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * current_pos_index, &position_buffer[0], GL_DYNAMIC_DRAW);
    GLint pos_attrib = glGetAttribLocation(shader->ID, "a_position");
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * current_col_index, &color_buffer[0], GL_DYNAMIC_DRAW);
    GLint col_attrib = glGetAttribLocation(shader->ID, "a_color");
    glEnableVertexAttribArray(col_attrib);
    glVertexAttribPointer(col_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, texcoord_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * current_tex_index, &texcoord_buffer[0], GL_DYNAMIC_DRAW);
    GLint tex_attrib = glGetAttribLocation(shader->ID, "a_texcoord");
    glEnableVertexAttribArray(tex_attrib);
    glVertexAttribPointer(tex_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * current_idx_index, &index_buffer[0], GL_DYNAMIC_DRAW);


    shader->Use();
    shader->SetMat4("u_proj", proj);
    shader->SetMat4("u_view", view);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, current_texture->GetID());
    shader->SetInt("u_texture", 0);

    //we divide by 3 as there are 3 values per index (x,y,z) or (r,g,b)
    glDrawElements(GL_TRIANGLES, current_idx_index, GL_UNSIGNED_INT, 0);
}

SpriteBatch::SpriteBatch()
{
    shader = std::make_unique<Shader>("Assets/Shaders/SpriteBatchVertex.glsl", "Assets/Shaders/SpriteBatchFragment.glsl");

    glGenBuffers(1, &position_vbo);
    glGenBuffers(1, &color_vbo);
    glGenBuffers(1, &texcoord_vbo);

    glGenBuffers(1, &index_vbo);
}

SpriteBatch::~SpriteBatch()
{
    glDeleteBuffers(1, &position_vbo);
    glDeleteBuffers(1, &color_vbo);
    glDeleteBuffers(1, &texcoord_vbo);

    glDeleteBuffers(1, &index_vbo);
}

    /**
     * @brief Sets the color for all following shapes.
     * 
     * @param color The color the following shapes will be. Ignores alpha channel
     */
    void SpriteBatch::SetColor(const glm::vec4& color)
    {
        this->current_color = glm::vec3(color.r, color.g, color.b);
    }

    void SpriteBatch::Begin(glm::mat4 proj, glm::mat4 view)
    {
        assert(!has_begun && "Spritebatch.end must be called before Spritebatch.begin");

        Clear();
        this->proj = proj;
        this->view = view;

        has_begun = true;
    }

void SpriteBatch::Draw(Texture const * const tex, float const x, float const y, float const w, float const h)
{
    
    if(current_texture == nullptr)
    {
        current_texture = tex;
    }
    if(tex != current_texture)
    {
        Flush();
        Clear();
    }

    if(current_pos_index >= POS_BUFFER_SIZE - 1)
    {
        Flush();
        Clear();
    }

    position_buffer[current_pos_index++] = x - w / 2.f;
    position_buffer[current_pos_index++] = y - h / 2.f;
    position_buffer[current_pos_index++] = 0;

    position_buffer[current_pos_index++] = x + w / 2.f;
    position_buffer[current_pos_index++] = y - h / 2.f;
    position_buffer[current_pos_index++] = 0;

    position_buffer[current_pos_index++] = x + w / 2.f;
    position_buffer[current_pos_index++] = y + h / 2.f;
    position_buffer[current_pos_index++] = 0;

    position_buffer[current_pos_index++] = x - w / 2.f;
    position_buffer[current_pos_index++] = y + h / 2.f;
    position_buffer[current_pos_index++] = 0;

    for(int i = 0; i < 4; i++)
    {
        color_buffer[current_col_index++] = current_color.r;
        color_buffer[current_col_index++] = current_color.g;
        color_buffer[current_col_index++] = current_color.b;
    }

    texcoord_buffer[current_tex_index++] = 0;
    texcoord_buffer[current_tex_index++] = 1;

    texcoord_buffer[current_tex_index++] = 1;
    texcoord_buffer[current_tex_index++] = 1;

    texcoord_buffer[current_tex_index++] = 1;
    texcoord_buffer[current_tex_index++] = 0;
    
    texcoord_buffer[current_tex_index++] = 0;
    texcoord_buffer[current_tex_index++] = 0;

    
    
    index_buffer[current_idx_index + 0] = current_vertex_index + 0; 
    index_buffer[current_idx_index + 1] = current_vertex_index + 1; 
    index_buffer[current_idx_index + 2] = current_vertex_index + 3; 

    index_buffer[current_idx_index + 3] = current_vertex_index + 3; 
    index_buffer[current_idx_index + 4] = current_vertex_index + 1; 
    index_buffer[current_idx_index + 5] = current_vertex_index + 2; 
    current_idx_index += 6;
    current_vertex_index += 4;
}

void SpriteBatch::End()
{
    assert(has_begun && "Spritebatch.end must be called before Spritebatch.begin");

    Flush();
    Clear();

    has_begun = false;
}
