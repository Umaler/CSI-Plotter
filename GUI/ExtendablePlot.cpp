#include "ExtendablePlot.hpp"

ExtendablePlot::ExtendablePlot() {
    set_size_request(left_reserve + right_reserve, up_reserve + down_reserve);
}

void ExtendablePlot::addDataSet(const DataSet& ds) {
    this->ds = &ds;
    ds.signalOnChanged().connect(sigc::mem_fun(*this, &ExtendablePlot::onUpdates));
}

void ExtendablePlot::onUpdates(const DataSet& updatedDS) {
    auto extr = updatedDS.getExtremums();

    maxX = extr.maxX;
    minX = extr.minX;
    maxY = extr.maxY;
    minY = extr.minY;

    queue_draw();
}

void ExtendablePlot::initShaders() {
    constexpr const char* const vertShaderProgram =     "#version 460\n"
                                                        "layout(location = 0) in vec2 aPos;\n"
                                                        "uniform double xMult;\n"
                                                        "uniform double xShift;\n"
                                                        "uniform double yMult;\n"
                                                        "uniform double yShift;\n"
                                                        "void main() {\n"
                                                        //"   gl_Position = vec4(0, 0, 0, 1);\n"
                                                        "   gl_Position = vec4(aPos.x * xMult + xShift, aPos.y * yMult + yShift, 0.0, 1.0);\n"
                                                        "}";

    constexpr const char* const fragShaderProgram =     "#version 460\n"
                                                        "out vec4 FragColor;\n"
                                                        "uniform vec4 color;\n"
                                                        "void main() {\n"
                                                        "   FragColor = color;\n"
                                                        "}";
    shader = Shader(vertShaderProgram, fragShaderProgram);

    constexpr const char textVertProg[] =  "#version 460\n"
                                           "layout (location = 0) in vec2 aPos;\n"
                                           "layout (location = 1) in vec2 aTexCoord;\n"
                                           "\n"
                                           "out vec2 TexCoord;\n"
                                           "\n"
                                           "void main()\n"
                                           "{\n"
                                               "gl_Position = vec4(aPos, 0.0, 1.0);\n"
                                               "TexCoord = aTexCoord;\n"
                                           "}";
    constexpr const char textFragProg[] =  "#version 460\n"
                                           "out vec4 FragColor;\n"
                                           "\n"
                                           "in vec2 TexCoord;\n"
                                           "\n"
                                           "uniform sampler2D ourTexture;\n"
                                           "\n"
                                           "void main()\n"
                                           "{\n"
                                               "FragColor = texture(ourTexture, TexCoord);\n"
                                           "}";
    textureShader = Shader(textVertProg, textFragProg);
}

void ExtendablePlot::on_realize() {
    GLArea::on_realize();
    initShaders();
}

ExtendablePlot::OpenglDSBuffers::OpenglDSBuffers(const DataSet& dataSet) {
    glCreateBuffers(1, &VBO);
    glNamedBufferStorage(VBO, dataSet.getSizeOfBuffer(), dataSet.getFirstElementAddress(), GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &VAO);

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(double) * 2);

    glVertexArrayAttribFormat(VAO, 0, 2, GL_DOUBLE, false, 0);  //sets format of attribute

    glVertexArrayAttribBinding(VAO, 0, 0);
}

void ExtendablePlot::OpenglDSBuffers::enable() {
    glBindVertexArray(VAO);
    glEnableVertexArrayAttrib(VAO, 0);
}

void ExtendablePlot::OpenglDSBuffers::disable() {
    glDisableVertexArrayAttrib(VAO, 0);
}

ExtendablePlot::OpenglDSBuffers::~OpenglDSBuffers() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

ExtendablePlot::OpenglCairoBuffer::OpenglCairoBuffer(const Cairo::ImageSurface& surface) {
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    glTextureStorage2D(texture, 1, GL_RGBA8, surface.get_width(), surface.get_height());

    glTextureSubImage2D(texture, 0, 0, 0, surface.get_width(), surface.get_height(), GL_BGRA, GL_UNSIGNED_BYTE, surface.get_data());

    float vertices[] = {
        -1.0, 1.0, 0.0, 0.0,
        -1.0, -1.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 0.0,
        1.0, -1.0, 1.0, 1.0
    };

    glCreateBuffers(1, &VBO);

    glNamedBufferStorage(VBO, sizeof(vertices), vertices, GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &VAO);

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(float) * 4);

    glVertexArrayAttribFormat(VAO, 0, 2, GL_FLOAT, false, 0);  //sets format of attribute
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, false, sizeof(float) * 2);  //sets format of attribute

    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribBinding(VAO, 1, 0);
}

void ExtendablePlot::OpenglCairoBuffer::enable() {
    glBindVertexArray(VAO);
    glEnableVertexArrayAttrib(VAO, 0);
    glEnableVertexArrayAttrib(VAO, 1);
    glBindTextureUnit(0, texture);
}

void ExtendablePlot::OpenglCairoBuffer::disable() {
    glDisableVertexArrayAttrib(VAO, 0);
    glDisableVertexArrayAttrib(VAO, 1);
}

ExtendablePlot::OpenglCairoBuffer::~OpenglCairoBuffer() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteTextures(1, &texture);
}

void ExtendablePlot::drawDataSet(const DataSet& data, Gdk::RGBA color, EdgePositions edgePos) {
    glBindVertexArray(0);
    OpenglDSBuffers buffer(data);

    glUseProgram(shader);
    buffer.enable();

    double xMult = (edgePos.right - edgePos.left) / (maxX - minX);
    double xShift = edgePos.left - xMult * minX;
    int xMultLoc = glGetUniformLocation(shader, "xMult");
    glUniform1d(xMultLoc, xMult);
    int xShiftLoc = glGetUniformLocation(shader, "xShift");
    glUniform1d(xShiftLoc, xShift);

    double yMult = (edgePos.up - edgePos.down) / (maxY - minY);
    double yShift = edgePos.down - yMult * minY;
    int yMultLoc = glGetUniformLocation(shader, "yMult");
    glUniform1d(yMultLoc, yMult);
    int yShiftLoc = glGetUniformLocation(shader, "yShift");
    glUniform1d(yShiftLoc, yShift);

    int colorLoc = glGetUniformLocation(shader, "color");
    glUniform4f(colorLoc, color.get_red(), color.get_green(), color.get_blue(), color.get_alpha());

    glDrawArrays(GL_LINE_STRIP, 0, data.getNumberOfPoints());
}

void ExtendablePlot::drawLegend(size_t windowWidth, size_t windowHeight, EdgePositions pos) {
    auto surface = Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, windowWidth, windowHeight);
    auto cr = Cairo::Context::create(surface);

    pos.down = -pos.down;  //because cairo's and opengl's ordinate
    pos.up   = -pos.up;    //coordinates are opposite

    pos.up *= windowHeight;//transform opengl's coordinate system
    pos.right *= windowWidth;//into cairo's one
    pos.down *= windowHeight;
    pos.left *= windowWidth;

    cr->set_source_rgba(1, 1, 1, 0);    //make white transparent background
    cr->paint();

    cr->translate(windowWidth / 2, windowHeight / 2);
    cr->scale(0.5, 0.5);

    cr->set_source_rgb(0, 0, 0);        //draw box around
    cr->set_line_width(4);
    cr->rectangle(pos.left, pos.down, pos.right - pos.left, pos.up - pos.down);
    cr->stroke();

    auto font = Cairo::ToyFontFace::create("", Cairo::ToyFontFace::Slant::NORMAL, Cairo::ToyFontFace::Weight::NORMAL);
    cr->set_font_face(font);
    cr->set_font_size(40);

    Cairo::TextExtents te;

    std::string maxYStr = std::to_string(static_cast<long long>(maxY));
    cr->get_text_extents(maxYStr, te);
    double maxYPosX = pos.left - te.width - 5;
    double maxYPosY = pos.up + te.height;
    cr->move_to(maxYPosX, maxYPosY);
    cr->show_text(maxYStr);

    std::string minYStr = std::to_string(static_cast<long long>(minY));
    cr->get_text_extents(minYStr, te);
    double minYPosX = pos.left - te.width - 5;
    double minYPosY = pos.down;
    cr->move_to(minYPosX, minYPosY);
    cr->show_text(minYStr);

    std::string minXStr = std::to_string(static_cast<long long>(minX));
    cr->get_text_extents(minXStr, te);
    double minXPosX = pos.left;
    double minXPosY = pos.down + te.height + 5;
    cr->move_to(minXPosX, minXPosY);
    cr->show_text(minXStr);

    std::string maxXStr = std::to_string(static_cast<long long>(maxX));
    cr->get_text_extents(maxXStr, te);
    double maxXPosX = pos.right - te.width;
    double maxXPosY = pos.down + te.height + 5;
    cr->move_to(maxXPosX, maxXPosY);
    cr->show_text(maxXStr);

    glUseProgram(textureShader);
    glBindVertexArray(0);
    OpenglCairoBuffer buffer(*surface);
    buffer.enable();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

bool ExtendablePlot::on_render(const Glib::RefPtr< Gdk::GLContext >& context) {
    Gdk::RGBA foreground(0.0, 0.0, 0.0, 1.0), background(1.0, 1.0, 1.0, 1.0);

    glBlendFunc(GL_SRC_COLOR,  GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );

    glClearColor(background.get_red(),
                 background.get_green(),
                 background.get_blue(),
                 background.get_alpha());
    glClear(GL_COLOR_BUFFER_BIT);

    double width = context->get_surface()->get_width();
    double height = context->get_surface()->get_height();
    EdgePositions graphBox {1.0 - up_reserve / height / 2.0,
                            1.0 - right_reserve / width / 2.0,
                            -1.0 + down_reserve / height / 2.0,
                            -1.0 + left_reserve / width / 2.0};

    auto lastMaxX = maxX;
    auto lastMinX = minX;
    auto lastMaxY = maxY;
    auto lastMinY = minY;
    if(ds->getNumberOfPoints() >= 3)
        drawDataSet(*ds, {1.0, 0.0, 0.0, 1.0}, graphBox);
    maxX = ds->getNumberOfPoints();
    minX = 0;
    if(maxY < 0) maxY = 0;
    if(minY > 0) minY = 0;
    drawLegend(context->get_surface()->get_width(), context->get_surface()->get_height(), graphBox);
    glFlush();

    maxX = lastMaxX;
    minX = lastMinX;
    maxY = lastMaxY;
    minY = lastMinY;

    int err = glGetError();
    if(err != GL_NO_ERROR) {
        std::cout << "Error: " << err << std::endl;
    }

    return true; //to stop other handlers from being invoked for the event
}
