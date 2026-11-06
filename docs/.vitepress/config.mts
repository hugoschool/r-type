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
                    { text: "Building", link: "/building" },
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
            },
            {
                text: "Protocol",
                items: [
                    { text: "Overview", link: "/protocol/overview" },
                    { text: "Communication example", link: "/protocol/example" },
                    { text: "Packets", link: "/protocol/packets" },
                ]
            },
            {
                text: "Client",
                items: [
                    { text: "Overview", link: "/client/overview" },
                    { text: "Architecture", link: "/client/architecture" },
                ]
            },
            {
                text: "Engine",
                items: [
                    { text: "ECS", link: "/engine/ecs" },
                    { text: "Architecture", link: "/engine/architecture" },
                    { text: "Display Module", link: "/engine/display_module" },
                    { text: "Sound Module", link: "/engine/sound_module" },
                    { text: "Asset Manager", link: "/engine/asset_manager" },
                    { text: "Configurations", link: "/engine/config" },
                    { text: "Scenes", link: "/engine/scenes" },
                    { text: "Subscribers", link: "/engine/subscribers" },
                ]
            },
            {
                text: "Server",
                items: [
                    { text: "Overview", link: "/server/overview" },
                    { text: "Architecture", link: "/server/architecture" },
                ]
            },
            {
                text: "Network",
                items: [
                    { text: "Usage", link: "/network/usage" },
                ]
            }
        ],

        socialLinks: [
            { icon: "github", link: "https://github.com/hugoschool/r-type" }
        ]
    }
});
