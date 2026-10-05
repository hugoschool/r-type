import { defineConfig } from "vitepress";

// https://vitepress.dev/reference/site-config
export default defineConfig({
    title: "R-Type",
    description: "A multi-threaded server and a graphical client video-game",
    themeConfig: {
        // https://vitepress.dev/reference/default-theme-config
        nav: [
            { text: "Home", link: "/" },
            { text: "Docs", link: "/introduction" }
        ],

        sidebar: [
            {
                text: "General",
                items: [
                    { text: "Introduction", link: "/introduction" },
                    { text: "Contributing", link: "/contributing" },
                ]
            },
            {
                text: "Comparative Study",
                items: [
                    { text: "Introduction", link: "/comparative/introduction" },
                    { text: "Packaging & Build Systems", link: "/comparative/package" },
                    { text: "ECS vs Mediator", link: "/comparative/ecs-mediator" },
                    { text: "Network library", link: "/comparative/network" },
                    { text: "Graphical library", link: "/comparative/graphical" },
                    { text: "Conclusion", link: "/comparative/conclusion" },
                ]
            }
        ],

        socialLinks: [
            { icon: "github", link: "https://github.com/hugoschool/r-type" }
        ]
    }
});
