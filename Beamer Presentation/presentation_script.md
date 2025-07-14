Let us begin by examining the motivation behind this thesis. I have always been passionate about video games—especially competitive ones. When I discovered Counter-Strike, it fundamentally changed my perspective. Over the years, I have accumulated more than two thousand hours playing legitimately, without ever resorting to cheating.

As I advanced through the competitive ranks and gained ELO, I started encountering increasingly suspicious opponents. This led me to review match replays and ask questions like: _“How did he know I was behind that wall?”_ These moments sparked my initial interest in the underlying mechanics of cheating. Like many players who witness questionable behavior in online games, my first reaction was curiosity: _How are they doing that?_

That curiosity soon evolved into a deeper exploration: _How is this technically possible? And why are they not being banned?_

Ultimately, this personal journey took shape as a clear academic objective—the very foundation of this thesis: to transform a personal passion into a rigorous technical investigation, contributing meaningful insights to both the academic field and the broader gaming community.

First, it is essential to define what _cheating_ means in the context of online games. Cheating refers to any unfair manipulation—whether through software or hardware—that grants a player an undue advantage over their opponents. This can include altering game mechanics, accessing hidden information, or automating actions to outperform other players.

Equally important is understanding the concept of _anti-cheat_ systems. These are dedicated software solutions designed to monitor player integrity, detect anomalous behavior, and take corrective action to preserve fair play. 

Cheating manifests in many forms, ranging from aimbots and wallhacks to visibility exploits, radar manipulation, and automated scripts. This constant evolution of cheating techniques creates a never-ending arms race between cheat developers and anti-cheat systems—each adapting to outpace the other.

Once we have defined what cheating is, the natural question becomes: _How did cheating originate?_ In its earliest form, cheating was not malicious—it emerged from debugging tools intentionally introduced by developers. A well-known example is the **Konami Code**, a sequence of inputs embedded into games to allow developers to test levels or mechanics without playing through the entire game.

As time progressed, external tools began to appear. One notable example is **GameShark**, a hardware device originally designed for the PlayStation and other consoles. It allowed users to intercept and modify memory values during gameplay, effectively granting advantages such as infinite lives, unlocked content, or enhanced stats.

A major turning point in the evolution of game cheating was the emergence of **Cheat Engine**, an open-source tool that enables users to scan and modify memory at runtime. Cheat Engine allows for pointer tracing, access manipulation, and the creation of reusable “cheat tables” that can be shared across the community—lowering the technical barrier for casual users while empowering more advanced memory analysis.

The year **2010** marked a turning point in the evolution of game cheating for several key reasons. First, **UnKnoWnCheaTs** reached its peak as a central hub for the game hacking community—a forum where users actively shared knowledge about reverse engineering, anti-cheat bypass techniques, memory patterns, and dumped detection modules.

At the same time, the distribution model for cheats began to shift. Developers started moving away from free public releases and embraced a **commercial model** based on private, subscription-based access. This transition led to the emergence of cheat providers offering regular updates, customer support, and tiered feature sets—effectively professionalizing the ecosystem.

Moving from its early origins to the present day, there has been a noticeable increase in the **commercialization and sophistication** of cheat software. Modern cheats have evolved into highly advanced products, incorporating **driver-level code**, **evasion techniques**, and **robust infrastructure**. Developers now invest in **code obfuscation**, **HWID spoofers**, and encrypted delivery systems to ensure persistence, stealth, and resistance against detection. These developments mirror the practices of professional software engineering, blurring the line between hobbyist tools and full-fledged commercial solutions.

We will now examine how anti-cheat systems have evolved over the years, highlighting key milestones in their development. Among the earliest was **PunkBuster**, widely recognized as the first commercial anti-cheat solution. It operated by capturing periodic screenshots of the player’s display and scanning for known cheat signatures, particularly those circulating freely within the community.

In contrast, modern anti-cheat systems have adopted far more advanced techniques. For example, **Vanguard**, developed by Riot Games, runs at the **kernel level**, granting it deep access to system processes in order to detect low-level cheats. Meanwhile, **VAC Live**, Valve’s latest iteration of its anti-cheat infrastructure, integrates **real-time detection** with **machine learning models** to analyze behavioral patterns and respond dynamically to emerging threats.

This progression reflects the growing technical arms race between cheat developers and anti-cheat technologies.

We will now discuss **VAC (Valve Anti-Cheat)** and its strategic approach to cheat detection. Since its introduction during the era of _Counter-Strike 1.6_, VAC has undergone continuous evolution. Notably, it operates entirely in **user mode**, a deliberate choice that reflects Valve’s commitment to respecting user privacy by avoiding invasive kernel-level access.

VAC employs a distinctive strategy centered on **delayed bans**. Rather than acting immediately upon detection, it gathers extensive information about the cheat, its behavior, and the specific injection techniques used. This intelligence-driven approach allows Valve to improve VAC’s detection capabilities while minimizing the likelihood of tipping off cheat developers. 

Recent updates have marked a significant shift with the integration of **AI-based detection**. The new system, referred to as **VAC Live**, incorporates machine learning models capable of identifying anomalous in-game behavior in real time, thereby enhancing responsiveness and adaptability.

Recent statistics highlight the widespread and harmful impact of cheating in online games:
- **60%** of players reported experiencing a negative impact due to cheating.
- **77%** said they would stop playing a game if cheating became too prevalent.
- **69%** support the implementation of permanent bans for cheaters.
- **55%** believe that developers are not doing enough to address the problem.
    
These results emphasize the urgent need for more robust and transparent anti-cheat measures, as cheating not only compromises fair play but also undermines long-term player engagement and trust.

Most cheats rely on **DLL injection** to insert their code into the game process and modify it directly in memory without significantly affecting performance. These are commonly referred to as **internal cheats**.

Modern cheat developers tend to avoid classic and easily detectable methods such as LoadLibrary or CreateRemoteThread. Instead, they employ **manual mapping**, a stealthier technique where the DLL is manually loaded into the target process’s memory space, its sections are mapped, and the entry point is executed—**all without registering the module or invoking suspicious API calls**. This approach minimizes traces and evades common anti-cheat detection heuristics.

VAC applies various countermeasures similar to those used by other anti-cheat systems. These include **signature scanning** of known cheat binaries, **inspection and dumping of suspicious threads**, and **detection of VMT (Virtual Method Table) hooks**—a technique that has become largely obsolete and is now commonly replaced by **trampoline hooking** for better stealth and reliability.

Many of these detection modules are embedded within steamservice.dll, a system component responsible for **loading and mapping VAC’s anti-cheat modules into memory** dynamically during runtime.

Hooking is one of the fundamental pillars of our cheat, alongside reverse engineering. The first step involves identifying key engine functions during the reverse engineering and debugging process. Once these functions are located, we intercept them using a hooking layer—in our case, the MinHook library, which offers simplicity and reliability.

For example, one of the functions we hook is responsible for rendering the smoke particles in the game. By intercepting this function, we can alter its behavior—such as disabling the rendering of smoke entirely or modifying its opacity—to gain a visual advantage.  

However, while MinHook is effective for prototyping and development, it is not the most stealthy method available. More advanced alternatives exist, such as Shadow Hooking, Page Guard hooking, or VEH (Vectored Exception Handler)-based hooks, which are harder to detect by modern anti-cheat systems. Depending on the threat model, these alternatives may be preferred in production-grade cheats.

The most important structure—and also the most challenging one to reverse engineer—is the **entity list**. Broadly speaking, it is the system the game uses to manage all active entities (such as players, weapons, and objects) in memory. In Counter-Strike, this entity list is handled dynamically through a handle-based paging system. Each entity is associated with a handle that is allocated across memory pages following a specific pattern, which we detail in the full document.

Due to time constraints, we will now showcase only a selection of the features we implemented.

The first is **Chams**. While the name might derive from “chameleon,” we are not entirely certain of its origin. The feature works by rendering player models with distinct visual materials to make them stand out through geometry. To achieve this, we began by decrypting Valve’s internal material files—specifically in the .vk3 format. By tracing rendering routines in the debugger, we identified the function responsible for creating materials. We then hooked this function, allowing us to programmatically generate our own materials and selectively apply them to entities of interest.

In addition, we reverse engineered the bone structure responsible for player physics and animations. During this process, we identified a **bone matrix** that is loaded into memory for each entity. Initially, we rendered the position of every individual bone in 3D space, as we did not yet know which index corresponded to each anatomical part. Once we visually mapped their positions, we implemented a skeleton rendering system by drawing lines between key bones—such as the head, spine, and limbs—to represent the player’s pose in real time.

We would have liked to cover other features such as the aimbot or sky/environment changer, but time constraints make it impossible within a 20-minute presentation.
To conclude, our final reflection is as follows:
- Cheating in online games reveals a complex interaction between technical creativity and social behavior.
- The constant battle between cheat developers and anti-cheat systems closely resembles broader cybersecurity challenges.
- Building cheats offers valuable offensive insight, exposing weaknesses in current detection methods.
- Addressing cheating effectively requires considering ethical, technical, and economic aspects together to preserve fair competition.

Looking ahead, there are several possible directions for future work:
- Automate schema extraction and offset updates to ensure long-term compatibility.
- Explore more stealthy hooking methods like hardware breakpoints or kernel indirection.
- Benchmark detection techniques using controlled test environments and real-time data.

