--это - Lua скрипт. С помощью него будет создаваться логика игры.

--print("[LUA] Hello")

--Это скрипт, с помощью которого выполняется перемещение параллелепипеда.

local entityIndex = 0 -- это индекс объекта, с которым мы будем иметь дело)
deltaX = 2
deltaY = 123
deltaZ = "blablabla"
logical_type = false

--input:HideCursor()
if type(jit) == 'table' then print(jit.version) end
selectedEntity = entityManager:SearchByIndex(entityIndex)

function Update()
    --Если зажали стрелку "вверх"
    if input:GetKeyDown(265) then
        --Получаем позицию объекта (в нашем случае параллелепипеда)
        newPosition = selectedEntity:GetPosition()
        --Изменяем позицию
        newPosition.z = newPosition.z - 0.5
        --Устанавливаем новую позицию
        selectedEntity:SetPosition(newPosition)
    end
    --Если зажали стрелку "вниз"
    if input:GetKeyDown(264) then
        newPosition = selectedEntity:GetPosition()
        newPosition.z = newPosition.z + 0.5
        selectedEntity:SetPosition(newPosition)
    end
    --Если зажали стрелку "влево"
    if input:GetKeyDown(263) then
        newPosition = selectedEntity:GetPosition()
        newPosition.x = newPosition.x - 0.5
        selectedEntity:SetPosition(newPosition)
    end
    --Если зажали стрелку "вправо"
    if input:GetKeyDown(262) then
        newPosition = selectedEntity:GetPosition()
        newPosition.x = newPosition.x + 0.5
        selectedEntity:SetPosition(newPosition)
    end
    
    --print(time:DeltaTime())
    --print(deltaX)
    --print("Index")
    --print(m_index)
    --print(selectedEntity:GetPosition()) --Выводим в консоль позицию объекта
    --io.write(m_index, selectedEntity:GetPosition());
    --newPosition = gameCamera:GetPosition()
    --if input:GetKeyDown(87) then     --W
    --   gameCamera:Front(1.0)
    --elseif input:GetKeyDown(83) then --S
    --    gameCamera:Back(1.0)
    --elseif input:GetKeyDown(65) then --A
    --    gameCamera:Left(1.0)
    --elseif input:GetKeyDown(68) then --D
    --    gameCamera:Right(1.0)
    --end

--    deltaX = input:GetDeltaX()
--    deltaY = input:GetDeltaY()

--    gameCamera:Yaw(deltaX)
--    gameCamera:Pitch(-deltaY)

end
