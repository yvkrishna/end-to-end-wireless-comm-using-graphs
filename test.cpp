#include <bits/stdc++.h>
#include <cstdio>
#include <iostream>

#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

using namespace std;

int main() {
    // ----------------- Init GLFW -----------------
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "10x10 Grid", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // ----------------- Init GLAD -----------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    // ----------------- Init ImGui -----------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = "unwanted/imgui.ini"; // move ini
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // ----------------- Grid state -----------------
    enum NodeType {NONE, SERVER, USER};
    NodeType nodeType[10][10] = {NONE};

    // Mode flags
    bool addServersMode = false;
    bool addUsersMode = false;
    bool serversConfirmed = false;
    bool usersConfirmed = false;

    // Data structures
    vector<pair<int,int>> servers;
    vector<pair<int,int>> users;

    // ----------------- Main loop -----------------
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ----------------- Main Window -----------------
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize| ImGuiWindowFlags_NoBackground;
        ImGui::Begin("10x10 Grid", nullptr, window_flags);

        // --- Top buttons ---
        ImGui::Text("Controls:");
        if (!serversConfirmed) {
            if (ImGui::Button("Add Servers")) addServersMode = !addServersMode;
            ImGui::SameLine();
        }
        if (!usersConfirmed) {
            if (ImGui::Button("Add Users")) addUsersMode = !addUsersMode;
        }

        ImGui::Separator();

        // --- Grid ---
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                ImGui::PushID(i*10+j);
                ImVec4 color(0.7f,0.7f,0.7f,1.0f); // default gray
                if (nodeType[i][j]==SERVER) color = ImVec4(0,1,0,1);
                if (nodeType[i][j]==USER) color = ImVec4(1,1,0,1);

                ImGui::PushStyleColor(ImGuiCol_Button, color);
                if (ImGui::Button("##", ImVec2(40,40))) {
                    if (addServersMode && !serversConfirmed) nodeType[i][j] = SERVER;
                    if (addUsersMode && !usersConfirmed) nodeType[i][j] = USER;
                }
                ImGui::PopStyleColor();
                ImGui::PopID();

                if (j < 9) ImGui::SameLine();
            }
        }

        ImGui::Separator();

        // --- Confirm buttons at bottom ---
        if (addServersMode && !serversConfirmed) {
            if (ImGui::Button("Confirm Servers")) {
                serversConfirmed = true;
                addServersMode = false;
                servers.clear();
                for (int i=0;i<10;i++) for (int j=0;j<10;j++)
                    if (nodeType[i][j]==SERVER) servers.push_back({i,j});
                
                cout << "Servers:\n"; for(auto &p: servers) cout << "("<<p.first<<","<<p.second<<") "; cout<<endl;
            }
        }

        if (addUsersMode && !usersConfirmed) {
            if (ImGui::Button("Confirm Users")) {
                usersConfirmed = true;
                addUsersMode = false;
                users.clear();
                for (int i=0;i<10;i++) for (int j=0;j<10;j++)
                    if (nodeType[i][j]==USER) users.push_back({i,j});
                
                cout << "\nUsers:\n"; for(auto &p: users) cout << "("<<p.first<<","<<p.second<<") ";cout<<endl;
            }
        }

        ImGui::End();

        // ----------------- Render -----------------
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0,0,display_w,display_h);
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // ----------------- Cleanup -----------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
