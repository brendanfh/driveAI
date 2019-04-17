local GAME_WIDTH = 160
local GAME_HEIGHT = 120

local loops = {}
local gates = {}

local STATES = {
    NOTHING = 0;
    TRACK_SELECTED = 1;
    ADDING_TRACK = 2;
    REWARD_SELECTED = 3;
    ADDING_REWARD = 4;
}
local state = STATES.NOTHING


function love.load()
    love.graphics.setBackgroundColor(0.4, 0.7, 0.9)
end

function love.update(dt)
    if love.keyboard.isDown "escape" then
        love.event.quit()
    end

    if love.keyboard.isDown "z" then
        if state == STATES.NOTHING then
            state = STATES.TRACK_SELECTED
        end
    end
end

function love.draw()
    love.graphics.setColor(1, 0, 0)
    for _, loop in ipairs(loops) do
        for i = 1, #loops - 1 do
            love.graphics.line(loop[i].x, line[i].y, line[i+1].x, line[i+1].y)
        end
        love.graphics.line(loop[#loop].x, loop[#loop].y, loop[1].x, loop[1].y)
    end

    love.graphics.setColor(0, 1, 0)
    for _, reward in ipairs(gates) do
        love.graphics.line(reward.x1, reward.y1, reward.x2, reward.y2)
    end
end