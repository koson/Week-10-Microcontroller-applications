cmake_minimum_required(VERSION 3.16)

# สำคัญ: ต้อง include project.cmake ของ ESP-IDF
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# ตั้งชื่อโปรเจ็กต์ (ตั้งชื่ออะไรก็ได้ที่เป็นตัวอักษร/ตัวเลข/ขีดล่าง)
project(esp_app)
