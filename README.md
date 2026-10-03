# ✈️ 飞机大战 PlaneBattle

Qt 6 写的桌面飞机射击游戏：单人闯关、双人合作、双人对战三种模式，带 Boss 战、道具系统与历史得分记录。

<p align="center">
  <img src="https://lixiaoshuai-git.github.io/assets/plane1.png" width="270" alt="主菜单">
  <img src="https://lixiaoshuai-git.github.io/assets/plane2.png" width="270" alt="玩法说明">
  <img src="https://lixiaoshuai-git.github.io/assets/plane3.png" width="270" alt="游戏画面">
</p>

---

## 🎮 玩法

| 模式 | 说明 |
|:---|:---|
| **单人模式** | 一名玩家挑战不断出现的敌机，以获取更高分数为目标 |
| **双人合作** | 两名玩家共同迎战敌机，共享屏幕，但生命值和得分各自独立计算，考验团队配合 |
| **双人对战** | VS 模式：玩家之间可以互相攻击，同时也要应对中立敌机，目标是击败对方玩家 |

**操作**

| 按键 | 功能 |
|:---|:---|
| `W` `A` `S` `D` 或小键盘 `8` `4` `5` `6` | 移动 |
| `E` 或 `0` | 发射子弹 |
| `Q` | 使用全屏炸弹 |

**道具**

| 道具 | 效果 |
|:---|:---|
| 回血道具 | 恢复 30 点生命值 |
| 武器升级 | 提升导弹等级，加快射速与伤害，最多 6 级 |
| 无敌道具 | 三秒无敌，免疫与敌机的碰撞 |

---

## 🛠️ 技术栈

- **框架**：Qt 6（Widgets + Multimedia）
- **构建**：CMake 3.16+ / C++17
- **资源**：通过 `res.qrc` 编译进可执行文件（CMake `AUTORCC`）

---

## 📂 目录结构

```
PlaneBattle/
├── main.cpp / mainwindow.*      # 程序入口与主窗口
├── widget.* / widget.ui         # 游戏主界面与状态管理
├── play01.* / play02.* / play03.*  # 单人 / 双人合作 / 双人对战 三条玩法逻辑
├── plane.*                      # 玩家飞机（移动、射击、升级）
├── enemy.* / enemybullet.*      # 普通敌机与敌机子弹
├── boss.* / boss2.*             # 两个 Boss
├── bossbullet.* / bossbullet2.* # Boss 弹幕
├── bullet.* / bomb.* / prop.*   # 我方子弹、炸弹、道具
├── map.* / music.*              # 滚动地图与音效
├── scoremanager.*               # 得分与历史记录
├── over.*                       # 结算界面
├── before.*                     # 最早的草稿实现（仍在源文件列表中）
├── config.h                     # 全局常量配置
├── res.qrc                      # 资源清单（图片 / 音效 / 图标）
└── CMakeLists.txt
```

---

## 🚀 构建运行

### 1️⃣ 准备资源

仓库里**不包含 `res/` 目录**（图片与音效约 20 MB，已从版本库移出），请先到 [Releases](../../releases) 下载 `res.zip`，解压到项目根目录，确认根目录下出现 `res/`：

```
PlaneBattle/
├── res/          ← 解压得到，含 app.ico、bg.wav、boss.png 等 51 个资源
├── res.qrc
└── CMakeLists.txt
```

> 缺少 `res/` 时 CMake 的 `AUTORCC` 会因为找不到 `res.qrc` 里列出的文件而报错。

### 2️⃣ 编译

```bash
cmake -B build -DCMAKE_PREFIX_PATH=<你的 Qt6 安装路径>
cmake --build build --config Release
```

Windows 上也可以直接用 **Qt Creator** 打开 `CMakeLists.txt`，选好 Kit 后点运行。

---

## 📝 待整理

- 可执行目标名目前是 `111`（`CMakeLists.txt` 里 `project(111 ...)`），建议改成 `PlaneBattle`
- `before.cpp` / `before.h` 是最早的草稿实现，可以删除并同步清理 `CMakeLists.txt`
- `tool.cpp` 是 0 字节空文件

---

## 📄 许可

本项目采用 [MIT 许可证](LICENSE) © 2026 苑泽宇 (Alan)。
