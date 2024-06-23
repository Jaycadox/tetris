const std = @import("std");
const raylib = @import("raylib");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const mode = b.standardOptimizeOption(.{});
   
    const r = raylib.addRaylib(b, target, mode, .{}) catch unreachable; 

    const game = b.addExecutable(.{
        .name = "game",
        .target = target,
        .optimize = mode,
    });
    game.linkLibC();
    game.addCSourceFiles(.{ 
        .files = &.{
            "main.c",
            "pallet_texture.c",
            "texture_store.c",
            "tetromino.c",
            "board.c",
            "score.c",
        },
        .flags = &.{
            "-std=c99",
            "-pedantic",
            "-Wall",
            "-W",
            "-Wno-missing-field-initializers",
            "-fno-sanitize=undefined",
            "-Wno-gnu-binary-literal",
        }
    });
    game.addIncludePath(.{.path = "raylib/src"});
    game.linkLibrary(r);
    b.installArtifact(game);
}
